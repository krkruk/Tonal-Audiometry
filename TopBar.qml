import QtQuick 2.0

Rectangle {
    id: topBarProto
    width: parent.width
    height: parent.height * 0.16
    color: barColor
    gradient: neutralBar

    antialiasing: true

    property color barColor: Qt.rgba(0,0,0, 0.4)
    property color menuBarColor: "white"
    property color textColor: "white"
    property color drawingColor: "white"
    property color menuButtonColorActive: Qt.rgba(8,8,8, 0.2)
    property string text: "Slide, to choose a channel"
    property string functionActionName: "Exit"  //possibilities: "Exit", "Save"
    property bool functionButtonActive: false

    signal menuButtonClicked();
    signal functionButtonClicked();

    onFunctionActionNameChanged: drawing.requestPaint()

    Row {
        Rectangle {
            id: menuAccessButton
            width: topBarProto.height
            height: topBarProto.height
            color: "transparent"

            property int itemHeight: height * 0.055
            property int itemWidth: width * 0.255
            property bool active: false

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
                onClicked: { menuButtonClicked();
                    if(menuAccessButton.active)
                        menuAccessButton.state = "onUnactive"
                    else
                        menuAccessButton.state = "onActive"
                    menuAccessButton.active = !menuAccessButton.active;
                }
            }

            states: [
                State {
                    name: "onUnactive"
                    PropertyChanges { target: menuAccessButton;
                        color: "transparent"; }
                },
                State {
                    name: "onActive"
                    PropertyChanges {
                        target: menuAccessButton
                        color: topBarProto.menuButtonColorActive
                    }
                }

            ]
        }
        Rectangle{
            id: spacer
            height: topBarProto.height
            width: 2
            color: topBarProto.menuButtonColorActive
        }

        Item {
            id: textBar
            width: {
                var tempWidth = topBarProto.width - menuAccessButton.width ;
                return topBarProto.functionButtonActive ?
                            tempWidth-menuAccessButton.width : tempWidth;
            }
            height: topBarProto.height
            Text{
                id: textBarContent
                text: topBarProto.text
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: font.pixelSize / 2
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.horizontalCenterOffset: -menuAccessButton.width / 2
                color: topBarProto.textColor
                font.pixelSize: parent.height * 0.6
            }
        }

        Rectangle {
            id: functionButton
            width: topBarProto.height
            height: topBarProto.height
            enabled: topBarProto.functionButtonActive
            visible: topBarProto.functionButtonActive
            gradient: releasedGradient

            Canvas {
                id: drawing
                anchors.centerIn: parent
                width: parent.width/4
                height: parent.height/4
                onPaint: {
                    var ctx = drawing.getContext("2d");
                    ctx.clearRect(0, 0, width, height);
                    ctx.beginPath();
                    ctx.strokeStyle = topBarProto.drawingColor;
                    ctx.lineWidth = 3;
                    ctx.fillStyle = ctx.strokeStyle;

                    if(topBarProto.functionActionName == "Save")
                    {
                        var h = height - 2;
                        ctx.moveTo(0, height);              //horizontal line
                        ctx.lineTo(width, h);
                        var horiHalf = width / 2;
                        var vertHalf = height / 2;
                        ctx.moveTo(horiHalf, 0);            //vertical line
                        ctx.lineTo(horiHalf, h);
                        ctx.moveTo(0, vertHalf);            //backslash line
                        ctx.lineTo(horiHalf, h);
                        ctx.lineTo(width, vertHalf);        //slash line
                        ctx.stroke();
                    } else
                    if(topBarProto.functionActionName == "Exit")
                    {
                        ctx.moveTo(0,0);
                        ctx.lineTo(width, height);
                        ctx.moveTo(width, 0);
                        ctx.lineTo(0, height);
                        ctx.stroke();
                    } else
                        console.log("Error: Invalid function request to draw");
                }

            }

            MouseArea{
                id: functionButtonMouseArea
                anchors.fill: parent
                onClicked: functionButtonClicked()
                onPressed: functionButton.state = "onPressed"
                onReleased: functionButton.state = "onReleased"
            }

            Gradient {
                id: pressedGradient
                GradientStop { color: Qt.rgba(8,8,8,0.1); position: 0.0}
                GradientStop { color: barColor; position: 0.3}
                GradientStop { color: barColor; position: 0.5}
                GradientStop { color: barColor; position: 0.7}
                GradientStop { color: Qt.rgba(8,8,8,0.1); position: 1.0}
            }
            Gradient {
                id: releasedGradient
                GradientStop { color: "transparent"; position: 1.0}
            }
            states: [
                State {
                    name: "onPressed"
                    PropertyChanges {
                        target: functionButton
                        gradient: pressedGradient
                    }
                },
                State {
                    name: "onReleased"
                    PropertyChanges {
                        target: functionButton
                        gradient: releasedGradient
                    }
                }
            ]
            transitions: [
                Transition {
                    from: "onPressed"; to: "onReleased"
                    PropertyAnimation {
                        property: "gradient"; duration: 100
                        easing.type: Easing.OutSine
                    }
                }
            ]
        }
    }

    Timer {
        id: neutralBarTimer
        interval: 500
        onTriggered: topBarProto.gradient = neutralBar
    }

    Gradient {
        id: neutralBar
        GradientStop { color: Qt.rgba(8,8,8,0.1); position: 0}
        GradientStop { color: barColor; position: 0.1}
        GradientStop { color: barColor; position: 0.9}
        GradientStop { color: Qt.rgba(8,8,8,0.1); position: 1}
    }

    Gradient {
        id: msgReceivedBar
        GradientStop { color: Qt.rgba(8,8,8,0.2); position: 0}
        GradientStop { color: barColor; position: 0.4}
        GradientStop { color: barColor; position: 0.5}
        GradientStop { color: barColor; position: 0.6}
        GradientStop { color: Qt.rgba(8,8,8,0.2); position: 1}
    }

    onTextChanged: {
        topBarProto.gradient = msgReceivedBar
        neutralBarTimer.start();
    }
}
