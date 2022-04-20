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
            source: "../assets/close_template.gif"
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
                Layout.preferredWidth: btn.btnWidthSmall; Layout.preferredHeight: btn.btnHeightSmall

                Image {
                    width: btn.btnWidthSmall; height: btn.btnHeightSmall
                    source: "../assets/buttons/buttonYes.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        closeTemplate_gif.source = "../assets/close_template_confirm.gif"
                        btn.clicks = btn.clicks + 1
                        if (btn.clicks == 2) {
                            btn.clicks = 0
                            stackView.replace("doorClose.qml")
                        }
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidthSmall; Layout.preferredHeight: btn.btnHeightSmall

                Image {
                    width: btn.btnWidthSmall; height: btn.btnHeightSmall
                    source: "../assets/buttons/buttonBack.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        btn.clicks = 0
                        stackView.replace("placementScreen.qml")
                    }
                }
            }

            RoundButton {
                Layout.preferredWidth: btn.btnWidthSmall; Layout.preferredHeight: btn.btnHeightSmall

                Image {
                    width: btn.btnWidthSmall; height: btn.btnHeightSmall
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
                Layout.preferredWidth: btn.btnWidthSmall; Layout.preferredHeight: btn.btnHeightSmall

                Image {
                    width: btn.btnWidthSmall; height: btn.btnHeightSmall
                    source: "../assets/buttons/buttonHelp.png"
                    fillMode: Image.PreserveAspectFit
                }

                TapHandler {
                    onTapped: {
                        closeTemplate_gif.source = "../assets/close_template.gif"
                        btn.clicks = 0
                        stackView.push("helpScreen.qml")
                    }
                }
            }
        }
    }
}