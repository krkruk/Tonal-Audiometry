import QtQuick 2.0

Rectangle {
    id: topBarProto
    width: parent.width
    height: parent.height * 0.16
    color: barColor

    property color barColor: "black"
    property color menuBarColor: "lightgray"
    property color textColor: "white"
    property string text: "Hello World"

    signal menuButtonClicked();

    Row {
        Item {
            id: menuAccessButton
            width: topBarProto.height
            height: topBarProto.height

            property int itemHeight: height * 0.12
            property int itemWidth: width * 0.7

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

