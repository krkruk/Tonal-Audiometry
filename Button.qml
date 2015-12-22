import QtQuick 2.0

Rectangle {
    id: buttonProto
    width: 200
    height: 300
    enabled: active
    visible: active

    gradient: Gradient {
        GradientStop { color: buttonColor; position: 0.0}
        GradientStop { color: Qt.rgba(16,16,16,0.2); position: 0.15}
        GradientStop { color: buttonColor; position: 1.0}
    }

    property string text: "Button"
    property color textColor: "white"
    property int textPixelSize: height / 3
    property color buttonColor: "black"
    property bool active: true
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

