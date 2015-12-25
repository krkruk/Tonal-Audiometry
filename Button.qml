import QtQuick 2.0

Rectangle {
    id: buttonProto
    width: 200
    height: 300
    enabled: active
    visible: active

    state: "onReleased"
    gradient: releasedGradient

    property string text: "Button"
    property color textColor: "white"
    property int textPixelSize: height / 3
    property color buttonColor: Qt.rgba(0,0,0, 0.4)
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
        onPressed: buttonProto.state = "onPressed"
        onReleased: buttonProto.state = "onReleased"
    }

    Gradient {
        id: pressedGradient
        GradientStop { color: Qt.rgba(8,8,8,0.1); position: 0}
        GradientStop { color: buttonColor; position: 0.3}
        GradientStop { color: buttonColor; position: 0.5}
        GradientStop { color: buttonColor; position: 0.7}
        GradientStop { color: Qt.rgba(8,8,8,0.1); position: 1}
    }
    Gradient {
        id: releasedGradient
        GradientStop { color: Qt.rgba(8,8,8,0.1); position: 0}
        GradientStop { color: buttonColor; position: 0.1}
        GradientStop { color: buttonColor; position: 0.9}
        GradientStop { color: Qt.rgba(8,8,8,0.1); position: 1}
    }

    states: [
        State{
            name: "onPressed"
            PropertyChanges {
                target: buttonProto
                gradient: pressedGradient
            }
        },
        State {
            name: "onReleased"
            PropertyChanges {
                target: buttonProto
                gradient: releasedGradient
            }
        }
    ]
    transitions: [
        Transition {
            from: "onPressed"; to: "onReleased"
            PropertyAnimation {
                property: "gradient"; duration: 100
                easing.type: Easing.OutSine
            }
        }

    ]
}

