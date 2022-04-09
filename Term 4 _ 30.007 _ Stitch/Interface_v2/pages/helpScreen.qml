import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: sew

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        GridLayout {
            anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            columns: 2
            rowSpacing: 25; columnSpacing: 50

            Item {
                id: btn4
                property var btnWidth: 324*1.5
                property var btnHeight: 52*1.5
            }

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
                Layout.columnSpan: 2
                text: "What kind of issues did you face?"
                font.family: "Helvetica Rounded"
                font.pointSize: 20
                color: "white"
            }

            RoundButton {
                Layout.preferredWidth: btn4.btnWidth; Layout.preferredHeight: btn4.btnHeight

                Image {
                    width: btn4.btnWidth; height: btn4.btnHeight
                    source: "../assets/buttons/buttonMinor.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {stackView.push("startScreen.qml")}
                }
            }

            RoundButton {
                Layout.preferredWidth: btn4.btnWidth; Layout.preferredHeight: btn4.btnHeight

                Image {
                    width: btn4.btnWidth; height: btn4.btnHeight
                    source: "../assets/buttons/buttonMajor.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {stackView.push("startScreen.qml")}
                }
            }

            RoundButton {
                Layout.preferredWidth: btn4.btnWidth/1.5; Layout.preferredHeight: btn4.btnHeight/1.5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
                Layout.columnSpan: 2

                Image {
                    width: btn4.btnWidth/1.5; height: btn4.btnHeight/1.5
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