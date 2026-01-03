#include "DbManager.h"

#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

DbManager::DbManager(QObject *parent)
    : QObject(parent)
{
}

DbManager& DbManager::instance()
{
    static DbManager inst;
    return inst;
}

bool DbManager::init()
{
    m_db = QSqlDatabase::addDatabase("QODBC");

    m_db.setDatabaseName(
        "Driver={PostgreSQL Unicode(x64)};"
        "Server=127.0.0.1;"
        "Port=5432;"
        "Database=postgres;"
        "Uid=postgres;"
        "Pwd=rnjstnsdn;"
        );

    if (!m_db.open()) {
        qWarning() << "[DbManager] PostgreSQL open failed:"
                   << m_db.lastError().text();
        return false;
    }

    qDebug() << "[DbManager] PostgreSQL Connected!";
    return createTables();
}

bool DbManager::createTables()
{
    QSqlQuery q(m_db);

    if (!q.exec(
            "CREATE TABLE IF NOT EXISTS menu ("
            "id SERIAL PRIMARY KEY,"
            "name VARCHAR(100),"
            "price INT,"
            "category VARCHAR(50),"
            "is_soldout BOOLEAN DEFAULT FALSE)"
            )) {
        qWarning() << q.lastError().text();
        return false;
    }

    if (!q.exec(
            "CREATE TABLE IF NOT EXISTS pos_tables ("
            "id INT PRIMARY KEY,"
            "name VARCHAR(50),"
            "kiosk_ip VARCHAR(20),"
            "is_active BOOLEAN DEFAULT TRUE)"
            )) {
        qWarning() << q.lastError().text();
        return false;
    }

    if (!q.exec(
            "CREATE TABLE IF NOT EXISTS orders ("
            "id SERIAL PRIMARY KEY,"
            "table_id INT,"
            "total INT,"
            "status VARCHAR(20),"
            "created_at TIMESTAMP DEFAULT NOW())"
            )) {
        qWarning() << q.lastError().text();
        return false;
    }

    if (!q.exec(
            "CREATE TABLE IF NOT EXISTS order_items ("
            "id SERIAL PRIMARY KEY,"
            "order_id INT,"
            "menu_id INT,"
            "menu_name VARCHAR(100),"
            "qty INT,"
            "price INT)"
            )) {
        qWarning() << q.lastError().text();
        return false;
    }

    qDebug() << "[DbManager] PostgreSQL tables ready";
    return true;
}

bool DbManager::insertTestOrder(int tableNo)
{
    if (!m_db.isOpen()) {
        qWarning() << "[DbManager] DB not open";
        return false;
    }

    m_db.transaction();

    QSqlQuery q(m_db);
    q.prepare(
        "INSERT INTO orders (table_id, total, status) "
        "VALUES (?, ?, ?) RETURNING id"
        );
    q.addBindValue(tableNo);
    q.addBindValue(12000);
    q.addBindValue("ordered");

    if (!q.exec() || !q.next()) {
        qWarning() << q.lastError().text();
        m_db.rollback();
        return false;
    }

    int orderId = q.value(0).toInt();

    QSqlQuery q2(m_db);
    q2.prepare(
        "INSERT INTO order_items (order_id, menu_id, menu_name, qty, price) "
        "VALUES (?, ?, ?, ?, ?)"
        );
    q2.addBindValue(orderId);
    q2.addBindValue(1);
    q2.addBindValue("Americano");
    q2.addBindValue(2);
    q2.addBindValue(4500);

    if (!q2.exec()) {
        qWarning() << q2.lastError().text();
        m_db.rollback();
        return false;
    }

    m_db.commit();
    qDebug() << "[DbManager] Test order created. id =" << orderId;
    return true;
}

void DbManager::printOrders()
{
    QSqlQuery q(m_db);
    q.exec("SELECT id, table_id, total, status, created_at FROM orders ORDER BY id DESC");

    while (q.next()) {
        qDebug()
        << q.value(0).toInt()
        << q.value(1).toInt()
        << q.value(2).toInt()
        << q.value(3).toString()
        << q.value(4).toString();
    }
}
