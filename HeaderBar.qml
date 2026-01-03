import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: header
    height: 60
    width: parent.width
    color: "#1E1E1E"

    property string title: ""

    // 🔹 왼쪽 슬롯
    Item {
        id: leftSlot
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.verticalCenter: parent.verticalCenter
    }

    // 🔹 제목
    Text {
        text: header.title
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 20
        font.bold: true
    }

    // 🔹 오른쪽 슬롯
    Item {
        id: rightSlot
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.verticalCenter: parent.verticalCenter
    }

    // 기본 뒤로가기 버튼 (왼쪽 슬롯에 기본 제공)
    Button {
        text: "←"
        parent: leftSlot
        onClicked: stackView.pop()
    }
}
