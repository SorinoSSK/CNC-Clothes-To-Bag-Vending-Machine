import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: end

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: end_gif
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../assets/complete.gif"
            cache: true
            //fillMode: Image.PreserveAspectFit
        }

        Item {
            //Template Locking
            Timer {
                id: complete_time
                interval: 3000; running: true
                onTriggered: {
                    end_gif.source = "../assets/open_template.gif"
                    option.visible = true
                }
            }

            Timer {
                id: remove_time
                interval: 5000
                onTriggered: {
                    end_gif.source = "../assets/thank.gif"
                    thank_time.start()
                }
            }

            Timer {
                id: thank_time
                interval: 5000
                onTriggered: {
                    btn.clicks = 0
                    stackView.replace("splashScreen.qml")
                }
            }
        }
    }

    Rectangle {
        id: option
        width: main.width; height: main.height
        color: "transparent"
        visible: false

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
                        end_gif.source = "../assets/open_template_confirm.gif"
                        btn.clicks = btn.clicks + 1
                        if (btn.clicks == 2) {
                            btn.clicks = 0
                            end_gif.source = "../assets/remove_shirt.gif"
                            option.visible = false
                            remove_time.start()
                        }
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
                        end_gif.source = "../assets/open_template.gif"
                        btn.clicks = 0
                        stackView.push("helpScreen.qml")
                    }
                }
            }
        }
    }
}