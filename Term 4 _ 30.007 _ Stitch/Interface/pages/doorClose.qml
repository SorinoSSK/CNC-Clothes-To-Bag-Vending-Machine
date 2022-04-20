import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: doorClose

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: doorClose_gif
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../assets/doorClose.gif"
            cache: true
            //fillMode: Image.PreserveAspectFit
        }
    }

    Item {
        id: doorC

        Timer {
            id: lock_time
            interval: 500; running: true
            onTriggered: {
                con.send("toggleSafety")
            }
        }

        Connections {
            target: con
            onArduinoRead: {
                if (ardReply == "95482") {stackView.replace("sewingScreen.qml")}
            }
        }
    }
}