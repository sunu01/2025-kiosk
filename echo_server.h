#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>   // ⭐ JSON Object 사용

class EchoServer : public QTcpServer {
    Q_OBJECT

    // 🔥 서버 상태를 QML에서 바로 바인딩하기 위한 property
    Q_PROPERTY(bool running READ isListening NOTIFY runningChanged)

public:
    explicit EchoServer(QObject *parent = nullptr);

    // QML에서 호출하는 서버 제어
    Q_INVOKABLE void startListening(quint16 port = 8080);
    Q_INVOKABLE void stopServer();

signals:
    // 서버 상태 변경 알림 (켜짐/꺼짐)
    void runningChanged();

    // 클라이언트 연결/해제
    void clientConnected(const QString &address, quint16 port);
    void clientDisconnected(const QString &address, quint16 port);

    // JSON 수신 알림
    void jsonReceived(const QString &senderAddress,
                      const QJsonDocument &doc);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    // 다중 클라이언트 관리
    QList<QTcpSocket*> m_clients;
};

#endif // ECHO_SERVER_H
