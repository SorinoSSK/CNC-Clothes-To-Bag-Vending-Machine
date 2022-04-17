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
            property var xCount: 0

            //Template Locking
            Timer {
                id: lock_time
                interval: 500; running: true
                onTriggered: {
                    con.send("closeTemplate")
                }
            }

            Connections {
                target: con
                onArduinoRead: {
                    //Check for "Template locked" feedback
                    if (ardReply == "95478") {
                        sew_gif.source = "../assets/fusing.gif"
                        //Move Top X
                        con.send("x95")
                    }
                    //Check for "Top X moved" feedback
                    else if (ardReply == "95479") {
                        //Move Top Y
                        if (xCount == 0) {
                            con.send("y622")
                            xCount = xCount +1
                        }
                        //Move Bottom Y after "Top X correction" feedback
                        else {
                            con.send("w20")
                            XCount = 0
                        }
                    }
                    //Check for "Top Y moved" feedback, Move Top X after 
                    else if (ardReply == "95480") {con.send("x62.8")}
                    //Check for "Bottom Y moved" feedback, Move Bottom X after 
                    else if (ardReply == "95482") {con.send("q50")}

                    //Check for "Bottom X moved" feedback, Lower needle after
                    else if (ardReply == "95481") {con.send("lowerNeedle")}
                    //Check for "Needle lowered" feedback, send "Stitch" command after
                    else if (ardReply == "95483") {con.send("stitch")}
                    //Check for "Stitched" feedback, send "Sew" command after
                    else if (ardReply == "95485") {con.send("sew")}
                    //Check for "Sewn" feedback, Raise needle after
                    else if (ardReply == "95486") {con.send("raiseNeedle")}

                    //Check for "Needle raised" feedback, Unlock template after
                    else if (ardReply == "95484") {
                        sew_gif.source = "../assets/templateUnlock.gif"
                        con.send("openTemplate")
                    }
                    //Check for "Template unlocked" feedback, change screen after
                    else if (ardReply == "95487") {
                        stackView.replace("endScreen.qml")
                    }
                }
            }
        }

        // Item {
        //     //Template Locking
        //     Timer {
        //         id: lock_time
        //         interval: 2000; running: true
        //         onTriggered: {
        //             sew_gif.source = "../assets/sewing.gif"
        //             sew_time.start()
        //         }
        //     }

        //     //Sewing Process
        //     Timer {
        //         id: sew_time
        //         interval: 4000;
        //         onTriggered: {
        //             sew_gif.source = "../assets/templateUnlock.gif"
        //             unlock_time.start()
        //         }
        //     }

        //     //Template Unlocking
        //     Timer {
        //         id: unlock_time
        //         interval: 2000;
        //         onTriggered: stackView.replace("endScreen.qml")
        //     }
        // }
    }
}