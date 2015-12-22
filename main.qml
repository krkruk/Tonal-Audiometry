import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Window {
    id: mainWindow
    visible: true
    width: 0.75 * height
    height: 640
    color: "black"
    title: "Tonal Audiometry"

    property int buttonsHeight: height * 0.15


    signal playSequence(int channel);
    signal stopPlaying();
    signal heardButtonClicked();
    signal saveFileRequest(string url);

    Item {
        id: main
        anchors.fill: parent

        Connections {
            target: appEngine
            onPlaylistEnded: {
                main.state = "stopState"
                audiogram.source = "image://audiogram/data"
                topBar.text = "Finished!"
            }
            onTopBarMsgChanged: topBar.text = appEngine.topBarMsg
        }

        TopBar{
            id: topBar
            anchors.top: parent.top
            width: mainWindow.width
            height: mainWindow.height * 0.075
//            text: appEngine.topBarMsg()
            onMenuButtonClicked: Qt.quit()
        }

        Flickable {
            id: audiogramArea
            width: mainWindow.height > mainWindow.width? mainWindow.width * 0.95 : mainWindow.height * 0.95
            height: width * 0.75
            z: -3
            anchors.centerIn: parent
            contentWidth: audiogram.width
            contentHeight: audiogram.height

            property bool areaClicked: false

            Image {
                id: audiogram
                width: audiogramArea.width
                height: audiogramArea.height
                antialiasing: true
                source: "image://audiogram/helloWorld"
            }
            MouseArea {
                id: flickMouseArea
                anchors.fill: parent
                onClicked: {
                    if(!audiogramArea.areaClicked){
                        audiogram.width *= 2;
                        audiogram.height *= 2;
                    }
                    else{
                        audiogram.width /= 2;
                        audiogram.height /= 2;
                    }
                    audiogramArea.areaClicked = !audiogramArea.areaClicked
                }
            }
        }

        Button {
            id: hearButton
            height: mainWindow.buttonsHeight
            width: mainWindow.width
            anchors.bottom: parent.bottom
            active: false
            text: "I hear it!"

            onClicked: heardButtonClicked()
        }

        ChannelSelect {
            id: channelSelect
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: mainWindow.width / 2
            height: mainWindow.buttonsHeight
            onChannelChanged: playButton.channelSelectId = channel
            onCurrentChannelName: topBar.text = channelName

        }

        Button {
            id: playButton
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: mainWindow.width / 2
            height: mainWindow.buttonsHeight
            state: "startPlayState"
            text: "Play!"

            property int channelSelectId: 0

            onClicked: {
                playSequence(channelSelectId)
                main.state = "playingState"
            }
        }

        states: [
            State {
                name: "playingState"
                PropertyChanges{
                    target: playButton
                    active: false
                }
                PropertyChanges{
                    target: channelSelect
                    active: false
                }
                PropertyChanges{
                    target: hearButton
                    active: true
                }
                PropertyChanges {
                    target: audiogram
                    source: "image://audiogram/blank"
                }
                PropertyChanges {
                    target: topBar
                    functionButtonActive: true
                    functionActionName: "Exit"
                    onFunctionButtonClicked: {
                        mainWindow.stopPlaying();
                        main.state = "stopState"
                    }
                }
                PropertyChanges {
                    target: flickMouseArea
                    enabled: false
                }
                PropertyChanges {
                    target: audiogram
                    width: audiogramArea.width
                    height: audiogramArea.height
                }
            },
            State {
                name: "stopState"
                PropertyChanges{
                    target: playButton
                    active: true
                }
                PropertyChanges{
                    target: channelSelect
                    active: true
                }
                PropertyChanges{
                    target: hearButton
                    active: false
                }
                PropertyChanges {
                    target: topBar
                    functionButtonActive: true
                    functionActionName: "Save"
                    onFunctionButtonClicked: {
                        console.log("SaveFile")
                        main.state = "saveFile"
                    }
                }
                PropertyChanges {
                    target: flickMouseArea
                    enabled: true
                }
                PropertyChanges {
                    target: audiogram
                    width: audiogramArea.width
                    height: audiogramArea.height
                }
                PropertyChanges {
                    target: audiogramArea
                    areaClicked: false
                }
            },
            State {
                name: "saveFile"
                PropertyChanges {
                    target: saveDialog
                    visible: true
                }
            },
            State {
                name: "fileRejected"
                PropertyChanges {
                    target: saveDialog
                    visible: false
                }
                PropertyChanges {
                    target: topBar
                    functionButtonActive: false
                    functionActionName: "Exit"
                }
            }

        ]
    }
    FileDialog {
        id: saveDialog
        title: qsTr("Please choose a file destination")
        folder: shortcuts.pictures
        visible: false
        selectFolder: false
        selectMultiple: false
        selectExisting: false
        nameFilters: [ "Image file (*.png)", "Image file (*.jpg)" ]

        onAccepted: {
            main.state = "saveFile";
            console.log("Url: " + saveDialog.fileUrl);
            mainWindow.saveFileRequest(saveDialog.fileUrl)
        }
        onRejected: {
            main.state = "fileRejected";
            appEngine.setTopBarMsg(qsTr("Discarded"))
            console.log("Rejected");
        }
    }

    Component.onCompleted: {
        topBar.text = "Slide, to choose a channel"
    }
}
