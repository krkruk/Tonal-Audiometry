import QtQuick 2.0

Rectangle {
    id: calibrationProto
    anchors.fill: parent
    z: 2
    color: Qt.rgba(0,0,0, 0.5)

    signal calibrationAccepted(int decibel)
    signal calibrationDeclined()
    signal calibrationPlay()

    Rectangle {
        id: calibrationWidget
        anchors.centerIn: parent
        width: parent.height / 2
        height: parent.width / 2
        color: "black"

        Column {
            Rectangle {
                width: calibrationWidget.width
                height: calibrationWidget.height / 3
                color: "black"
                Text {
                    anchors.fill: parent
                    text: qsTr('<p align="center"><h1>Calibration</h1></p><p align="justify">This is 65 dB HL sound at 1000Hz.
                    Rub your hands energically and try to match the volume. If needed, you
may adjust it by sliding the value below.</p>')
                    wrapMode: Text.Wrap
                    color: "white"
                    font.pixelSize: 10
                }
            }
            Item {
                width: calibrationWidget.width
                height: 100
                ListView {
                    id: dbList
                    spacing: 5
                    anchors.centerIn: parent
                    width: 100
                    height: 100
                    model: ListModel {
                        ListElement { value: -10}
                        ListElement { value: -9}
                        ListElement { value: -8}
                        ListElement { value: -7}
                        ListElement { value: -6}
                        ListElement { value: -5}
                        ListElement { value: -4}
                        ListElement { value: -3}
                        ListElement { value: -2}
                        ListElement { value: -1}
                        ListElement { value: 0}
                        ListElement { value: 1}
                        ListElement { value: 2}
                        ListElement { value: 3}
                        ListElement { value: 4}
                        ListElement { value: 5}
                        ListElement { value: 6}
                        ListElement { value: 7}
                        ListElement { value: 8}
                        ListElement { value: 9}
                        ListElement { value: 10}
                    }
                    delegate: Rectangle{
                        width: 100
                        height: 100
                        gradient: releasedGradient

                        property int dbValue: value


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
                            color: "White"
                            font.pixelSize: parent.height / 3
                            text: parent.dbValue
                        }
                    }
                    snapMode: ListView.SnapOneItem
                    highlightRangeMode: ListView.StrictlyEnforceRange
                    clip: true
                    Component.onCompleted: dbList.positionViewAtIndex(10, ListView.Center)
                }
            }

            Row {
                Button {
                    id: decline
                    width: calibrationWidget.width / 2
                    height: calibrationWidget.height / 4
                    text: "Decline"
                    onClicked: {
                        calibrationProto.calibrationDeclined()
                        calibrationProto.destroy()
                    }
                }
                Button {
                    id: accept
                    width: calibrationWidget.width / 2
                    height: calibrationWidget.height / 4
                    text: "Accept"
                    onClicked: {
                        calibrationProto.calibrationAccepted(dbList.currentItem.dbValue)
                        calibrationProto.destroy()
                    }
                }
            }
        }

        Component.onCompleted: calibrationPlay();
    }
}
