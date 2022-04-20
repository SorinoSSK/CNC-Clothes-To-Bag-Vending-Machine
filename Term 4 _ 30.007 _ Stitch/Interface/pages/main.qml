import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


ApplicationWindow {
    id: main
    visible: true
    visibility: "FullScreen"
    width: 960; height: 600
    title: "Stitch"

    Item {
        id: btn
        property var btnWidth: 243
        property var btnHeight: 39
        property var btnWidthSmall: 202.5
        property var btnHeightSmall: 32.5
        property var clicks: 0

        property var rSpacing: 20
        property var cSpacing: 40

        Timer {
            interval: 50; running: true
                onTriggered: {con.send("getTemperature")}
        }
        
        Connections {
            target: con
        }
    }

    StackView {
        id: stackView
        initialItem: "splashScreen.qml"
    }

    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: Qt.BlankCursor
    }
}