import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 640
    color: "black"

    signal playSequence();
    signal heardButtonClicked();

    Connections {
        target: appEngine
        onPlaylistEnded: playButton.state = "startPlayState"
    }

    TopBar{
        id: topBar
        anchors.top: parent.top
        width: mainWindow.width
        height: mainWindow.height * 0.075
    }
    Image{
        source: "image://audiogram"
        width: parent.width
        height: parent.height/2
        anchors.centerIn: parent
    }

    Button {
        id: quitButton
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: mainWindow.width / 2
        height: 100
        text: qsTr("Quit")
        onClicked: Qt.quit()
    }

        Button {
            id: playButton
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: mainWindow.width / 2
            height: 100
            state: "startPlayState"

            states: [
                State{
                    name: "startPlayState"
                    PropertyChanges {
                        target: playButton
                        text: qsTr("Play")
                        onClicked: {
                            mainWindow.playSequence();
                            state = "playingState";
                        }
                    }
                },
                State{
                    name: "playingState"
                    PropertyChanges {
                        target: playButton
                        text: qsTr("I hear the sound")
                        onClicked: {
                            mainWindow.heardButtonClicked();
                        }
                    }
                }
            ]
        }

}
