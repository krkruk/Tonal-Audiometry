import QtQuick 2.0

Rectangle {
    id: topBarProto
    width: parent.width
    height: parent.height * 0.16
    color: barColor
    gradient: Gradient {
        GradientStop { color: barColor; position: 0.0}
        GradientStop { color: Qt.rgba(16,16,16,0.2); position: 0.85}
        GradientStop { color: barColor; position: 1.0}
    }

    antialiasing: true

    property color barColor: "black"
    property color menuBarColor: "white"
    property color textColor: "white"
    property string text: "Hello World"

    signal menuButtonClicked();

    Row {
        Item {
            id: menuAccessButton
            width: topBarProto.height
            height: topBarProto.height

            property int itemHeight: height * 0.055
            property int itemWidth: width * 0.255

            Column {
                spacing: iH
                anchors.centerIn: parent

                property alias iH: menuAccessButton.itemHeight
                property alias iW: menuAccessButton.itemWidth

                Rectangle {width:parent.iW; height: parent.iH; color: topBarProto.menuBarColor}
                Rectangle {width:parent.iW; height: parent.iH; color: topBarProto.menuBarColor}
                Rectangle {width:parent.iW; height: parent.iH; color: topBarProto.menuBarColor}
            }

            MouseArea {
                id: menuAccessButtonMouseArea
                anchors.fill: parent
                onClicked: menuButtonClicked();
            }
        }
        Rectangle{
            id: spacer
            height: topBarProto.height
            width: 2
            gradient: Gradient {
                GradientStop { color: "black"; position: 0.0}
                GradientStop { color: Qt.rgba(64,64,64,0.2); position: 0.85}
                GradientStop { color: "black"; position: 1.0}
            }
        }

        Item {
            id: textBar
            width: topBarProto.width - menuAccessButton.width
            height: topBarProto.height
            Text{
                id: textBarContent
                text: topBarProto.text
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -menuAccessButton.width / 2
                color: topBarProto.textColor
                font.pixelSize: parent.height * 0.75
            }
        }
    }
}

