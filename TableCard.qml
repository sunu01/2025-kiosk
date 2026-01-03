import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: card
    width: 180
    height: 120
    radius: 10

    // 🔹 외부에서 제어할 속성
    property int tableNumber: 1
    property bool occupied: false
    property string statusText: ""

    // 🔹 클릭 이벤트 전달
    signal clicked(int tableNumber)

    color: occupied ? "#E74C3C" : "#2ECC71"

    Column {
        anchors.centerIn: parent
        spacing: 8

        Text {
            text: "Table " + card.tableNumber
            color: "white"
            font.pixelSize: 18
            font.bold: true
        }

        Text {
            text: statusText !== ""
                  ? statusText
                  : (occupied ? "사용 중" : "비어 있음")
            color: "white"
            font.pixelSize: 14
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: card.clicked(card.tableNumber)
    }
}
