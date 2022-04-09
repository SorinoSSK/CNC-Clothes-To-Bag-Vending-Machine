import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: setting

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        GridLayout {
            anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            columns: 1
            rows: 4
            rowSpacing: 25; columnSpacing: 50

            Item {
                id: btn5
                property var btnWidth: 324*1
                property var btnHeight: 52*1
            }

            Text {
                id: textOutput
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: "Text Output"
                font.family: "Helvetica Rounded"
                font.pointSize: 20
                color: "white"

                Connections {
                    target: con
                    onArduinoRead: {textOutput.text = ardReply}
                }
            }

            TextField {
                id: textInput
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: 200; height: 100
                font.pixelSize: 20
                placeholderText: "Text Input"
                selectByMouse: true
                onAccepted: {
                    con.send(textInput.text)
                    textInput.text = ""
                    textOutput.text = ""
                }
            }

            RoundButton {
                Layout.preferredWidth: btn5.btnWidth; Layout.preferredHeight: btn5.btnHeight

                Image {
                    width: btn5.btnWidth; height: btn5.btnHeight
                    source: "../assets/buttons/buttonBack.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {stackView.pop()}
                }
            }
        }
    }
}