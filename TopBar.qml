import QtQuick 2.0

Rectangle {
    id: topBarProto
    width: parent.width
    height: parent.height * 0.16
    color: barColor
    gradient: Gradient {
        GradientStop { color: barColor; position: 0.0}
        GradientStop { color: Qt.rgba(16,16,16,0.2); position: 0.85}
        GradientStop { color: barColor; position: 1.0}
    }

    antialiasing: true

    property color barColor: "black"
    property color menuBarColor: "white"
    property color textColor: "white"
    property color drawingColor: "white"
    property string text: "Hello World"
    property string functionActionName: "Exit"  //possibilities: "Exit", "Save"
    property bool functionButtonActive: false

    signal menuButtonClicked();
    signal functionButtonClicked();

    onFunctionActionNameChanged: drawing.requestPaint()

    Row {
        Item {
            id: menuAccessButton
            width: topBarProto.height
            height: topBarProto.height

            property int itemHeight: height * 0.055
            property int itemWidth: width * 0.255

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
                onClicked: menuButtonClicked();
            }
        }
        Rectangle{
            id: spacer
            height: topBarProto.height
            width: 2
            gradient: Gradient {
                GradientStop { color: "black"; position: 0.0}
                GradientStop { color: Qt.rgba(64,64,64,0.2); position: 0.85}
                GradientStop { color: "black"; position: 1.0}
            }
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
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -menuAccessButton.width / 2
                color: topBarProto.textColor
                font.pixelSize: parent.height * 0.6
            }
        }

        Item {
            id: functionButton
            width: topBarProto.height
            height: topBarProto.height
            enabled: topBarProto.functionButtonActive
            visible: topBarProto.functionButtonActive

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
            }
        }


    }
}
