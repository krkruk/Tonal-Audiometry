import QtQuick 2.0

Rectangle {
    id: menuProto
    width: mainWindow.width / 2
    height: mainWindow.height - topBar.height - buttonsHeight
    anchors.top: parent.top
    anchors.topMargin: topBar.height
    anchors.left: parent.left
    focus: true
    enabled: false
    visible: false
    opacity: 0.0
    color: Qt.rgba(0,0,0, 0.95);

    property bool active: false

    signal exitedMenuComponent()
    signal calibDeclined()
    signal calibAccepted(int decibel)
    signal calibPlay()

    Item {
        id: menuItem
        anchors.fill: parent

        ListModel {
            id: listModel
            ListElement { name: "Calibration"; file: "Calibration.qml" }
            ListElement { name: "About"; file: "About.qml" }
        }

        ListView {
            id: listView
            anchors.fill: parent
            model: listModel
            delegate: Component {
                Button {
                    width: menuProto.width
                    height: topBar.height
                    text: name
                    onClicked: {
                        var component = Qt.createComponent(file);
                        var obj = component.createObject(mainWindow)
                        if(name=="Calibration")
                        {
                            obj.calibrationDeclined.connect (menuProto.calibDeclined);
                            obj.calibrationAccepted.connect(menuProto.calibAccepted)
                            obj.calibrationPlay.connect(menuProto.calibPlay)
                        }

                        exitedMenuComponent()
                        menuProto.active = false;
                    }
                }
            }
            footer: Column{
                    Rectangle {
                        width: menuProto.width
                        height: 1
                        color: "white"
                        opacity: 0.5
                        antialiasing: true
                    }
                    Button {
                    width: menuProto.width
                    height: topBar.height * 1.1
                    text: qsTr("Exit")
                    onClicked: Qt.quit()
                }
            }
        }
    }

    onActiveChanged: {
        if(active)
            state = "onLoad"
        else
            state = "onExit"
    }

//    onOpacityChanged: if(opacity == 0 &&
//                              !onExitTransition.running) {
//                          enabled = false;
//                          visible = false;
//                      }

    states: [
        State {
            name: "onLoad"
            PropertyChanges {
                target: menuProto
                opacity: 1.0
                enabled: true
                visible: true
            }
        },
        State {
            name: "onExit"
            PropertyChanges {
                target: menuProto
                opacity: 0.0
                enabled: false
                visible: false
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "onLoad"
            PropertyAnimation {
                properties: "opacity"; duration: 350
                easing.type: Easing.OutSine
            }
        },
        Transition {
            id: onExitTransition
            from: "*"; to: "onExit"
            PropertyAnimation {
                properties: "opacity"; duration: 350
                easing.type: Easing.OutSine
            }
        }
    ]
}
