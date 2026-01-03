#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "DbManager.h"
#include "TableManager.h"
#include "echo_server.h"

int main(int argc, char *argv[])
{
    // 1. 기본 앱 생성
    QGuiApplication app(argc, argv);

    // 2. DB 초기화
    if (!DbManager::instance().init()) {
        qWarning() << "[Main] DB init failed. Exit.";
        return -1;
    }
    qDebug() << "[DB] Drivers:" << QSqlDatabase::drivers();

    // 3. QML 엔진 생성
    QQmlApplicationEngine engine;

    // 4. 핵심 객체 생성
    TableManager tableManager;
    EchoServer server;

    // 🔥🔥🔥 핵심 연결 (이게 없어서 안 됐던 것)
    QObject::connect(
        &server,
        &EchoServer::jsonReceived,
        &tableManager,
        [&](const QString &sender, const QJsonDocument &doc) {

            Q_UNUSED(sender);

            if (!doc.isObject()) {
                qWarning() << "[Main] JSON is not object";
                return;
            }

            QJsonObject obj = doc.object();

            // tableId 키 방어
            int tableId = -1;
            if (obj.contains("tableId"))
                tableId = obj["tableId"].toInt();
            else if (obj.contains("tabled"))
                tableId = obj["tabled"].toInt();

            qDebug() << "[Main] parsed tableId =" << tableId;

            QString menu    = obj["menu"].toString();
            int quantity    = obj["quantity"].toInt();
            int price       = obj["price"].toInt();

            qDebug() << "[Main] calling addOrder:"
                     << tableId << menu << quantity << price;

            tableManager.addOrder(tableId, menu, quantity, price);
        }
        );

    // 5. QML에 노출
    engine.rootContext()->setContextProperty("tableManager", &tableManager);
    engine.rootContext()->setContextProperty("echoServer", &server);

    // 6. QML 로드
    const QUrl url(QStringLiteral("qrc:/KioskServer1/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );

    engine.load(url);

    // 7. 초기 상태 반영
    tableManager.refresh();

    return app.exec();
}
