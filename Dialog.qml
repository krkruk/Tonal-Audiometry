import QtQuick 2.0

Rectangle {
    id: dialogPopup
    anchors.fill: parent
    z: 2
    color: Qt.rgba(0,0,0, 0.5)

    property string text: ""
    property color textColor: "white"

    Rectangle {
        id: rect
        width: parent.height / 2
        height: parent.width / 2
        anchors.centerIn: parent
        color: "black"

        TextEdit {
            id: textEdit
            anchors.fill: parent
            color: textColor
            readOnly: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.RichText
            text: dialogPopup.text
        }
    }

    MouseArea {
        id: aboutMouseArea
        anchors.fill: parent
        onClicked: dialogPopup.destroy()
    }
}
