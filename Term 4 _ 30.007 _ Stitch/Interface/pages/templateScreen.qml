import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: closeTemplate

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: closeTemplate_gif
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../assets/instructions/close_template.gif"
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
            spacing: 28

            Item {
                id: btn2
                property var btnWidth: 324/5*4
                property var btnHeight: 52/5*4
                property var clicks: 0
            }

            RoundButton {
                Layout.preferredWidth: btn2.btnWidth; Layout.preferredHeight: btn2.btnHeight

                Image {
                    width: btn2.btnWidth; height: btn2.btnHeight
                    source: "../assets/buttons/buttonYes.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        closeTemplate_gif.source = "../assets/instructions/close_template_confirm.gif"
                        btn2.clicks = btn2.clicks + 1
                        if (btn2.clicks == 2) {
                            stackView.replace("sewingScreen.qml")
                        }
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn2.btnWidth; Layout.preferredHeight: btn2.btnHeight

                Image {
                    width: btn2.btnWidth; height: btn2.btnHeight
                    source: "../assets/buttons/buttonBack.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn2.clicks = 0
                        stackView.replace("placementScreen.qml")
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn2.btnWidth; Layout.preferredHeight: btn2.btnHeight

                Image {
                    width: btn2.btnWidth; height: btn2.btnHeight
                    source: "../assets/buttons/buttonQuit.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn2.clicks = 0
                        stackView.replace("startScreen.qml")
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn2.btnWidth; Layout.preferredHeight: btn2.btnHeight

                Image {
                    width: btn2.btnWidth; height: btn2.btnHeight
                    source: "../assets/buttons/buttonHelp.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn2.clicks = 0
                        stackView.push("helpScreen.qml")
                    }
                }
            }
        }
    }
}