import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 640
    color: "black"

    signal playSequence(int channel);
    signal heardButtonClicked();

    Connections {
        target: appEngine
        onPlaylistEnded: {
            playButton.state = "startPlayState"
            audiogram.source = "image://audiogram/data"
        }
        onTopBarMsgChanged: {
            topBar.text = topBarMsg
        }
    }

    TopBar{
        id: topBar
        anchors.top: parent.top
        width: mainWindow.width
        height: mainWindow.height * 0.075
        onMenuButtonClicked: Qt.quit()
    }
    Image{
        id: audiogram
        source: "image://audiogram/helloWorld"
        width: parent.width
        height: parent.height/2
        anchors.centerIn: parent
    }

    ChannelSelect {
        id: channelSelect
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: mainWindow.width / 2
        height: 100
        onChannelChanged: playButton.channelSelectId = channel
        onCurrentChannelName: topBar.text = channelName

    }

        Button {
            id: playButton
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: mainWindow.width / 2
            height: 100
            state: "startPlayState"

            property int channelSelectId: 0

            states: [
                State{
                    name: "startPlayState"
                    PropertyChanges {
                        target: playButton
                        text: qsTr("Play")
                        onClicked: {
                            mainWindow.playSequence(channelSelectId);
                            state = "playingState";
                            audiogram.source = "image://audiogram/blank"
                        }
                    }
                },
                State{
                    name: "playingState"
                    PropertyChanges {
                        target: playButton
                        text: qsTr("I hear the it!")
                        onClicked: {
                            mainWindow.heardButtonClicked();
                        }
                    }
                }
            ]
        }

}
