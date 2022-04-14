import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: splash

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: splash_gif
            source: "../assets/splash.gif"
            cache: true
            fillMode: Image.PreserveAspectFit
            TapHandler {
                onTapped: {
                    stackView.replace("startScreen.qml")
                }
            }
        }
    }
}