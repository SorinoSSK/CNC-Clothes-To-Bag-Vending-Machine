import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: sew

    Rectangle {
        width: main.width; height: main.height
        color: "#131926"

        AnimatedImage {
            id: sew_gif
            source: "../assets/templateLock.gif"
            cache: true
            fillMode: Image.PreserveAspectFit
        }

        Item {
            //Template Locking
            Timer {
                id: lock_time
                interval: 2000; running: true
                onTriggered: {
                    sew_gif.source = "../assets/process.gif"
                    sew_time.start()
                }
            }

            //Sewing Process
            Timer {
                id: sew_time
                interval: 4000;
                onTriggered: {
                    sew_gif.source = "../assets/templateUnlock.gif"
                    unlock_time.start()
                }
            }

            //Template Unlocking
            Timer {
                id: unlock_time
                interval: 2000;
                onTriggered: stackView.replace("endScreen.qml")
            }
        }
    }
}