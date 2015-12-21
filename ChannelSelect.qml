import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle {
    id: channelSelectProto
    width: 100
    height: 100
    color: "black"

    property color textColor: "white"
    property int textPixelSize: height / 3

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
                color: channelSelectProto.color
                property string buttonText: name
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
