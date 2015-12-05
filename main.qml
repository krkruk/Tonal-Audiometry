import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 640

    signal playSequence();

    Column{
        spacing: 20

        Rectangle{
            id: quitButton
            width: mainWindow.width
            height: 100

            Text {
                text: qsTr("Quit");
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea{
                anchors.fill: parent
                onClicked: Qt.quit()
            }
        }

        Rectangle{
            id: playButton
            width: mainWindow.width
            height: 100

            Text {
                text: qsTr("Play");
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea{
                anchors.fill: parent
                onClicked: mainWindow.playSequence();
            }
        }
    }
}
