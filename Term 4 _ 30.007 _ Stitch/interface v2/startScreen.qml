import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: start

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: start_gif
            anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top;
            source: "../assets/start.gif"
            cache: true
            fillMode: Image.PreserveAspectFit
        }

        Button {
            id: admin_button
            anchors.top: parent.top; anchors.right: parent.right

            Image {
                id: admin_button_img
                anchors.fill: parent
                source: "../assets/buttons/buttonAdmin.png"
                fillMode: Image.PreserveAspectFit
            }

            TapHandler {
                onTapped: {
                    stackView.push("settingScreen.qml")
                }
            }
        }

        Rectangle {
            id: start_button
            anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top;

            AnimatedImage {
                id: start_button_gif
                anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top; anchors.topMargin: main.height/4*3;
                source: "../assets/buttons/buttonStart.gif"
                cache: true
                fillMode: Image.PreserveAspectFit

                TapHandler {
                    onTapped: {
                        stackView.replace("placementScreen.qml")
                    }
                }
            }
        }
    }
}