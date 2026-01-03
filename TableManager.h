#pragma once

#include <QObject>
#include <QVariantList>
#include <QMap>

class TableManager : public QObject
{
    Q_OBJECT

    // 🔥 QML GridView용 테이블 목록
    Q_PROPERTY(QVariantList tableList READ tableList NOTIFY tableListChanged)

public:
    explicit TableManager(QObject *parent = nullptr);

    // GridView 모델
    QVariantList tableList() const;

    // DetailPage 주문 목록
    Q_INVOKABLE QVariantList getOrders(int tableId);

    // 주문 추가 (서버 / 테스트 공용)
    Q_INVOKABLE void addOrder(int tableId,
                              const QString &menu,
                              int quantity,
                              int price);

    // 강제 갱신
    Q_INVOKABLE void refresh();

    // 결제 완료 → 테이블 초기화
    Q_INVOKABLE void clearTable(int tableId);

signals:
    void tableListChanged();
    void ordersChanged(int tableId);

private:
    int m_tableCount = 6;            // 테이블 6개 고정
    QVariantList m_tableList;        // 카드용 상태
    QMap<int, QVariantList> m_orders; // tableId → 주문 목록

    void loadTableStates();
};
