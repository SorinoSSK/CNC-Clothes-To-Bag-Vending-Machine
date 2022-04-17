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
            source: "../assets/clothe.gif"
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
            spacing: btn.rSpacing

            RoundButton {
                Layout.preferredWidth: btn.btnWidth; Layout.preferredHeight: btn.btnHeight

                Image {
                    width: btn.btnWidth; height: btn.btnHeight
                    source: "../assets/buttons/buttonYes.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        placement_gif.source = "../assets/clothe_confirm.gif"
                        btn.clicks = btn.clicks + 1
                        if (btn.clicks == 2) {
                            btn.clicks = 0
                            stackView.replace("templateScreen.qml")
                        }
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidth; Layout.preferredHeight: btn.btnHeight

                Image {
                    width: btn.btnWidth; height: btn.btnHeight
                    source: "../assets/buttons/buttonQuit.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn.clicks = 0
                        stackView.replace("startScreen.qml")
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidth; Layout.preferredHeight: btn.btnHeight

                Image {
                    width: btn.btnWidth; height: btn.btnHeight
                    source: "../assets/buttons/buttonHelp.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        placement_gif.source = "../assets/clothe.gif"
                        btn.clicks = 0
                        stackView.push("helpScreen.qml")
                    }
                }
            }
        }
    }
}