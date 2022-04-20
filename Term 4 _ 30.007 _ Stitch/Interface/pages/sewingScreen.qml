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
            id: temp
            property var cut: false
            property var step_stamp: 0
            property var step_cut: 0

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
                    if (ardReply == "95481") {
                        sew_gif.source = "../assets/warning.gif"
                    }

                    else {
                        //STAMPING
                        if (temp.cut == false) {
                            console.log(ardReply)
                            temp.step_stamp = temp.step_stamp + 1
                            console.log(temp.step_stamp)
                            //Check for "Template locked" feedback
                            if (ardReply == "95475" && temp.step_stamp == 1) {
                                sew_gif.source = "../assets/fusing.gif"
                                con.send("x95 t200")
                            }
                            //Check for "Top X moved" feedback, Move Top Y
                            else if (ardReply == "95479" && temp.step_stamp == 2) {con.send("y622")}
                            //Check for "Top Y moved" feedback, Top X correction
                            else if (ardReply == "95479" && temp.step_stamp == 3) {con.send("x62.8")}
                            //Check for "Top X moved" correction, Lower Heat Block
                            else if (ardReply == "95479" && temp.step_stamp == 4) {con.send("lowerHeatBlock")}
                            //Check for "Heat Block lowered" feedback, send "Stamp" command after
                            else if (ardReply == "95483" && temp.step_stamp == 5) {con.send("stamp")}
                            //Check for "Stamped" feedback, Raise Heat Block after
                            else if (ardReply == "95485" && temp.step_stamp == 6) {con.send("homeTopZ")}
                            //Check for "Heat Block raised" feedback, Move Top X, Cool down
                            else if (ardReply == "95490" && temp.step_stamp == 7) {con.send("x-500 t0")}
                            //Check for "Top X moved" feedback, Home Top Y
                            else if (ardReply == "95479" && temp.step_stamp == 8) {con.send("homeTopY")}
                            //Check for "Top Y homed" feedback, home Top X after
                            else if (ardReply == "95487" && temp.step_stamp == 9) {
                                temp.cut = true
                                temp.step_stamp = 0
                                con.send("homeTopX")
                            }
                            else if (ardReply == "95482") {
                                temp.step_stamp = temp.step_stamp - 1
                                sew_gif.source = "../assets/fusing.gif"
                            }
                            else {temp.step_stamp = temp.step_stamp - 1}
                        }

                        //Cutting
                        else {
                            console.log(ardReply)
                            temp.step_cut = temp.step_cut + 1
                            console.log(temp.step_cut)
                            //if (ardReply == "95475" && temp.step_cut == 1){
                            //    sew_gif.source = "../assets/fusing.gif"
                            //    con.send("x95")
                            //}
                            //After Homing Top X, Move Top X
                            if (ardReply == "95486" && temp.step_cut == 1){con.send("x95")}
                            //After Top X finished moving, Move Top Y
                            else if (ardReply == "95479" && temp.step_cut == 2) {con.send("y200")}
                            //After Top Y finished moving, Move Top X
                            else if (ardReply == "95479" && temp.step_cut == 3) {con.send("x60")}
                            //After Top X finished moving, Lower Cutter
                            else if (ardReply == "95479" && temp.step_cut == 4) {con.send("lowerCutter")}
                            //After Cutter Lowered, Move Top Y
                            else if (ardReply == "95478" && temp.step_cut == 5) {con.send("y250")}
                            //After Cutting, Raise Cutter
                            else if (ardReply == "95479" && temp.step_cut == 6) {con.send("raiseCutter")}
                            //After Cutter Raised, Move Top Y
                            else if (ardReply == "95477" && temp.step_cut == 7) {con.send("y-250")}
                            //After Top Y finished moving, Move Top X to 2nd cutting zone
                            else if (ardReply == "95479" && temp.step_cut == 8) {con.send("x490")}
                            //After Reaching 2nd cutting zone, Lower Cutter
                            else if (ardReply == "95479" && temp.step_cut == 9) {con.send("lowerCutter")}
                            //After Cutter Lowered, Move Top Y
                            else if (ardReply == "95478" && temp.step_cut == 10) {con.send("y250")}
                            //After Cutting, Raise Cutter
                            else if (ardReply == "95479" && temp.step_cut == 11) {con.send("raiseCutter")}
                            //After Cutter Raised, Move Top X
                            else if (ardReply == "95477" && temp.step_cut == 12) {con.send("x-550")}
                            //After Top X finished moving, Home Top Y
                            else if (ardReply == "95479" && temp.step_cut == 13) {con.send("homeTopY")}
                            //After Homing Top Y, Home Top X
                            else if (ardReply == "95487" && temp.step_cut == 14) {con.send("homeTopX")}

                            //Check for "Top X homed" feedback, unlock template after
                            else if (ardReply == "95486" && temp.step_cut == 15) {
                                sew_gif.source = "../assets/templateUnlock.gif"
                                con.send("openTemplate")
                            }
                            //Check for "Template unlocked" feedback, change screen after
                            else if (ardReply == "95476" && temp.step_cut == 16) {
                                temp.step_cut = 0
                                con.send("toggleSafety")
                                stackView.replace("endScreen.qml")
                            }
                            else if (ardReply == "95482") {
                                temp.step_cut = temp.step_cut - 1
                                sew_gif.source = "../assets/fusing.gif"
                            }
                            else {temp.step_cut = temp.step_cut - 1}
                        }
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