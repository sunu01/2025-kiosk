import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: serverPage
    anchors.fill: parent

    // 🔹 공통 헤더바
    HeaderBar {
        id: header
        title: "서버 관리"
        anchors.top: parent.top
    }

    Rectangle {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#1E1E1E"

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: echoServer.running
                      ? "서버 동작 중 (8080)"
                      : "서버가 꺼져 있습니다."
                color: "white"
                font.pixelSize: 20
            }


            Row {
                spacing: 20

                Button {
                    text: "서버 시작"
                    onClicked: echoServer.startListening(8080)
                }

                Button {
                    text: "서버 중지"
                    onClicked: echoServer.stopServer()
                }

            }

            // 🔹 연결된 클라이언트 목록
            ListView {
                id: clientList
                width: 300
                height: 200
                model: clientModel
                clip: true

                delegate: Text {
                    text: model.display
                    color: "white"
                }
            }
        }
    }

    // 🔹 Model: 클라이언트 목록
    ListModel { id: clientModel }

    // 🔹 C++ -> QML 이벤트 수신
    Connections {
        target: echoServer

        onClientConnected: {
            clientModel.append({ display: address + ":" + port })
        }

        onClientDisconnected: {
            for (var i = 0; i < clientModel.count; i++) {
                if (clientModel.get(i).display.startsWith(address)) {
                    clientModel.remove(i)
                    break
                }
            }
        }
    }
}
