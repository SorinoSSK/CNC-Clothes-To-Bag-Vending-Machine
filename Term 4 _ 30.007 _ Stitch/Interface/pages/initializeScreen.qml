import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: reset

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: initializeScreen
            anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top;
            source: "../assets/initialize.gif"
            cache: true
            fillMode: Image.PreserveAspectFit
        }

        Item {
            id: booting

            Timer {
                id: readTemp
                interval: 10000; running: true
                onTriggered: {con.send("openTemplate")
                console.log("open")}
            }

            Connections {
                target: con
                onArduinoRead: {
                    if (ardReply == "95476") {con.send("homeTopY")}
                    else if (ardReply == "95487") {con.send("homeTopX")}
                    else if (ardReply == "95486") {con.send("homeTopZ")}
                    //else if (ardReply == "95490") {con.send("homeBottomX")}
                    //else if (ardReply == "95488") {con.send("homeBottomY")}

                    //else if (ardReply == "95489") {con.send("q250 w10")}
                    //else if (ardReply == "95481") {stackView.replace("placementScreen.qml")}
                    else if (ardReply == "95490") {stackView.replace("placementScreen.qml")}
                }
            }
        }
    }
}