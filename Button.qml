import QtQuick 2.0

Rectangle {
    id: buttonProto
    width: 200
    height: 300
    color: "black"

    property string text: "Button"
    property color textColor: "white"
    property int textPixelSize: height / 3
    signal clicked();

    Text {
        id: buttonText
        anchors.horizontalCenter: buttonProto.horizontalCenter
        anchors.verticalCenter: buttonProto.verticalCenter
        color: buttonProto.textColor
        font.pixelSize: buttonProto.textPixelSize
        text: buttonProto.text
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        onClicked: buttonProto.clicked()
    }
}

