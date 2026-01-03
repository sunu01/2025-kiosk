#pragma once

#include <QObject>
#include <QSqlDatabase>

class DbManager : public QObject
{
    Q_OBJECT
public:
    // 전역에서 하나만 쓰는 싱글톤 스타일
    static DbManager& instance();

    // DB 여는 함수 (오늘 목표)
    bool init(); //db열고 테이블 생성

    bool insertMenu(const QString &name, int price, const QString &category);
    void printMenus();
    bool insertTestOrder(int tableNo);
    void printOrders();

private:
    explicit DbManager(QObject *parent = nullptr);

    bool createTables();

    QSqlDatabase m_db;
};
