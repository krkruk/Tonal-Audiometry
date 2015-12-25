import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
    id: channelSelectProto
    width: 100
    height: 100
    enabled: active
    visible: active

    property bool active: true
    property color textColor: "white"
    property int textPixelSize: height / 3
    property color buttonColor: Qt.rgba(0,0,0, 0.4)

    signal channelChanged(int channel);
    signal currentChannelName(string channelName);

    ListModel {
        id: channelSelectModel

        ListElement { value: 0; name: "Left Channel" }
        ListElement { value: 1; name: "Right Channel" }
        ListElement { value: 2; name: "All Channels" }
    }

    ListView {
        id: channelList
        anchors.fill: parent
        model: channelSelectModel
        delegate:
            Rectangle{
                width: channelSelectProto.width
                height: channelSelectProto.height
                gradient: releasedGradient

                property string buttonText: name

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
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.gradient = pressedGradient
                        timer.start()
                    }
                }
                Timer {
                    id: timer
                    interval: 500
                    onTriggered: parent.gradient = releasedGradient
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: channelSelectProto.textColor
                    font.pixelSize: channelSelectProto.textPixelSize
                    text: buttonText
                }
            }

        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        clip: true
        onCurrentIndexChanged: channelChanged(currentIndex)
        onCurrentItemChanged: currentChannelName(currentItem.buttonText)
    }


}
