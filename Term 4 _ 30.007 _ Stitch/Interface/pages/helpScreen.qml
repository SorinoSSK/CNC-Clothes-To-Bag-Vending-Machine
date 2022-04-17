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
            rowSpacing: btn.rSpacing; columnSpacing: btn.cSpacing

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
                Layout.columnSpan: 2
                text: "What kind of issues did you face?"
                font.family: "Helvetica Rounded"
                font.pointSize: 14
                color: "white"
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidth / 4 * 5; Layout.preferredHeight: btn.btnHeight / 4 * 5

                Image {
                    width: btn.btnWidth / 4 * 5; height: btn.btnHeight / 4 * 5
                    source: "../assets/buttons/buttonMinor.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {stackView.push("startScreen.qml")}
                }
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidth / 4 * 5; Layout.preferredHeight: btn.btnHeight / 4 * 5

                Image {
                    width: btn.btnWidth / 4 * 5; height: btn.btnHeight / 4 * 5
                    source: "../assets/buttons/buttonMajor.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {stackView.push("startScreen.qml")}
                }
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidth; Layout.preferredHeight: btn.btnHeight
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
                Layout.columnSpan: 2

                Image {
                    width: btn.btnWidth; height: btn.btnHeight
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