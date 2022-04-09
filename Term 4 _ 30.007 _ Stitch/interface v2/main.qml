import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1


ApplicationWindow {
    id: main
    visible: true
    visibility: "FullScreen"
    width: 1280; height: 800
    title: "Stitch"

    StackView {
        id: stackView

        //initialItem: "settingScreen.qml"
        initialItem: "splashScreen.qml"
    }
}