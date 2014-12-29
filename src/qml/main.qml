import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true

    title: "Feather 0.1"

    menuBar: MainMenu {}

    toolBar: MainToolBar {}

    statusBar: StatusBar {
        RowLayout { BusyIndicator { implicitWidth: 10; implicitHeight: 10; running: true } Label { text: "Feather 0.1" } }
    }

    Viewport3D { anchors.fill: parent }

    Outliner {}

    Material {}

}
