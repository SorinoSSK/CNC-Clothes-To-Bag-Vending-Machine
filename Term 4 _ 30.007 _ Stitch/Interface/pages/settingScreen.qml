import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


Page{
    id: setting

    Rectangle {
        id: login
        width: main.width; height: main.height
        color: "#131926"
        visible: true

        Item {
            id: check
            function checkLogin(usr, pw) {
                if (usr == "admin" && pw == "123123") {
                    login.visible = false;
                    settings.visible = true;
                }
                else {
                    error.text = "Wrong Login"
                }
            }
        }

        GridLayout {
            anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            columns: 2; rows: 3
            rowSpacing: 25; columnSpacing: 25

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: "Username"
                font.family: "Helvetica Rounded"
                font.pointSize: 16
                color: "white"
            }

            TextField {
                id: user
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: 200; height: 100
                font.pixelSize: 16
                selectByMouse: true
                text: ''

                onAccepted: {check.checkLogin(user.text, pass.text)}
            }

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: "Password"
                font.family: "Helvetica Rounded"
                font.pointSize: 16
                color: "white"
            }

            TextField {
                id: pass
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                echoMode: TextInput.Password
                width: 200; height: 100
                font.pixelSize: 16
                selectByMouse: true
                text: ''

                onAccepted: {check.checkLogin(user.text, pass.text)}
            }

            Text {
                id: error
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.columnSpan: 2
                text: ''
                font.family: "Helvetica Rounded"
                font.pointSize: 14
                color: "red"
            }
        }
    }

    Rectangle {
        id: settings
        width: main.width; height: main.height
        color: "#131926"
        visible: false

        Item {
            id: values

            property var done: true

            Timer {
                id: readTemp
                interval: 3000; running: true; repeat: true
                onTriggered: {con.send("T")}
            }

            Connections {
                target: con
                onArduinoRead: {
                    if (values.substring(ardReply, 0, 1) == "T"){
                        temp.text = values.substring(ardReply, 1, 4)
                    }

                    else {values.done = True}
                }
            }

            function substring(str, start, end) {
                return str.substring(start, end);
            }
        }

        RoundButton {
            anchors.left: parent.left; anchors.bottom: parent.bottom
            width: btn.btnWidth; height: btn.btnHeight
            anchors.leftMargin: 50; anchors.bottomMargin: 50

            Image {
                width: btn.btnWidth; height: btn.btnHeight
                source: "../assets/buttons/buttonQuit.png"
                fillMode: Image.PreserveAspectFit
            }

            TapHandler {
                onTapped: {Qt.quit()}
            }
        }

        GridLayout {
            id: controls
            anchors.left: parent.left; anchors.top: parent.top
            anchors.leftMargin: 75; anchors.topMargin: 50
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            columns: 2; rows: 4
            rowSpacing: 25; columnSpacing: 25

            Text {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
                Layout.columnSpan: 2
                text: "Controls"
                font.family: "Helvetica Rounded"
                font.pointSize: 14
                color: "white"
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                checkable: true
                text: checked ? "Needle: Lowered" : "Needle: Raised"

                onReleased: {checked? con.send("raiseNeedle") : con.send("lowerNeedle")}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                checkable: true
                text: checked ? "Cutter: Lowered" : "Cutter: Raised"

                onReleased: {checked? con.send("CR") : con.send("CL")}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                text: "Home Top X"

                onClicked: {if(values.done = true){values.done = false ;con.send("HomeTopX")}}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                text: "Home Bottom X"

                onClicked: {if(values.done = true){values.done = false; con.send("HomeBottomX")}}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                text: "Home Top Y"

                onClicked: {if(values.done = true){values.done = false; con.send("HomeTopY")}}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                text: "Home Bottom Y"

                onClicked: {if(values.done = true){values.done = false; con.send("HomeBottomY")}}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                text: "Home Top Z"

                onClicked: {if(values.done = true){values.done = false; con.send("HomeTopZ")}}
            }

            RoundButton {
                implicitWidth: btn.btnWidthSmall; implicitHeight: btn.btnHeightSmall
                checkable: true
                text: checked ? "Template: Locked" : "Template: Unlocked"

                onReleased: {checked? con.send("openTemplate") : con.send("closeTemplate")}
            }
        }

        GridLayout {
            anchors.right: parent.right; anchors.top: parent.top
            anchors.rightMargin: 75; anchors.topMargin: 50
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            columns: 2; rows: 4
            rowSpacing: 25; columnSpacing: 25


            Text {
                text: "Temp"
                font.family: "Helvetica Rounded"
                font.pointSize: 16
                color: "white"
            }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: 200; height: 40
                color: "white"

                Text {
                    id: temp
                    anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                    text: ''
                }
            }
        }
    }

    Rectangle{
        width: main.width; height: main.height
        color: "transparent"

        RoundButton {
            anchors.right: parent.right; anchors.bottom: parent.bottom
            width: btn.btnWidth; height: btn.btnHeight
            anchors.rightMargin: 50; anchors.bottomMargin: 50

            Image {
                width: btn.btnWidth; height: btn.btnHeight
                source: "../assets/buttons/buttonBack.png"
                fillMode: Image.PreserveAspectFit
            }

            TapHandler {
                onTapped: {stackView.pop()}
            }
        }
    }
}