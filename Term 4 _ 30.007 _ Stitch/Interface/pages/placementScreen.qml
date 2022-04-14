import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: placement

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: placement_gif
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../assets/instructions/place_clothes_on_template.gif"
            cache: true
            //fillMode: Image.PreserveAspectFit
        }
    }

    Rectangle {
        width: main.width; height: main.height
        color: "transparent"

        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: parent.bottom; anchors.bottomMargin: main.height/10;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom;
            spacing: 20

            Item {
                id: btn1
                property var btnWidth: 324
                property var btnHeight: 52
                property var clicks: 0
            }

            RoundButton {
                Layout.preferredWidth: btn1.btnWidth; Layout.preferredHeight: btn1.btnHeight

                Image {
                    source: "../assets/buttons/buttonYes.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        placement_gif.source = "../assets/instructions/place_clothes_on_template_confirm.gif"
                        btn1.clicks = btn1.clicks + 1
                        if (btn1.clicks == 2) {
                            btn1.clicks = 0
                            stackView.replace("templateScreen.qml")
                        }
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn1.btnWidth; Layout.preferredHeight: btn1.btnHeight

                Image {
                    source: "../assets/buttons/buttonQuit.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn1.clicks = 0
                        stackView.replace("startScreen.qml")
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn1.btnWidth; Layout.preferredHeight: btn1.btnHeight

                Image {
                    source: "../assets/buttons/buttonHelp.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn1.clicks = 0
                        stackView.push("helpScreen.qml")
                    }
                }
            }
        }
    }
}