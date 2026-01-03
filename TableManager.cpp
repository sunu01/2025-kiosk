#include "TableManager.h"

#include <QVariantMap>
#include <QDateTime>
#include <QDebug>

TableManager::TableManager(QObject *parent)
    : QObject(parent)
{
    loadTableStates();
    qDebug() << "[TableManager] Initialized, table count:" << m_tableList.size();
}

// ===============================
// Q_PROPERTY READ
// ===============================
QVariantList TableManager::tableList() const
{
    return m_tableList;
}

// ===============================
// 테이블 상태 재계산 (중앙 통제)
// ===============================
void TableManager::loadTableStates()
{
    m_tableList.clear();

    for (int i = 1; i <= m_tableCount; ++i) {
        QVariantMap table;
        table["id"] = i;

        int total = 0;
        QVariantList orders = m_orders.value(i);

        for (const QVariant &v : orders) {
            QVariantMap o = v.toMap();
            total += o["price"].toInt() * o["quantity"].toInt();
        }

        table["total"] = total;
        table["busy"]  = (total > 0);

        m_tableList.append(table);
    }

    emit tableListChanged();
}

void TableManager::refresh()
{
    loadTableStates();
}

// ===============================
// 특정 테이블 주문 목록
// ===============================
QVariantList TableManager::getOrders(int tableId)
{
    return m_orders.value(tableId);
}

// ===============================
// 주문 추가
// ===============================
void TableManager::addOrder(int tableId,
                            const QString &menu,
                            int quantity,
                            int price)
{
    if (tableId < 1 || tableId > m_tableCount) {
        qWarning() << "[TableManager] Invalid tableId:" << tableId;
        return;
    }

    QVariantMap order;
    order["menu"]     = menu;
    order["quantity"] = quantity;
    order["price"]    = price;
    order["time"]     = QDateTime::currentDateTime()
                        .toString("HH:mm:ss");

    m_orders[tableId].append(order);

    qDebug() << "[TableManager] Order added:"
             << "table" << tableId
             << menu << quantity << price;

    loadTableStates();
    emit ordersChanged(tableId);
}

// ===============================
// 테이블 초기화 (결제 완료)
// ===============================
void TableManager::clearTable(int tableId)
{
    m_orders[tableId].clear();

    qDebug() << "[TableManager] Table cleared:" << tableId;

    loadTableStates();
    emit ordersChanged(tableId);
}
