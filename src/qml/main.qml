/***********************************************************************
 *
 * Filename: main.qml 
 *
 * Description: Entry point for the qml main window. 
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import feather.scenegraph 1.0

ApplicationWindow {
    id: window
    width: 1200
    height: 700
    visible: true
    title: "Feather 0.1"
    //flags: Qt.FramelessWindowHint

    Properties { id: properties }

    menuBar: MainMenu { properties: properties }
    
    NodeFields { id: fields }

    //toolBar: MainToolBar {}

    SplitView {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: timeEditor.top
        orientation: Qt.Horizontal

        SceneGraphEditor{
            id: sgEditor
            width: 200
            properties: properties
            fieldModel: fields
        }

        // temp viewport till I can get vulkan working in qml
        Rectangle {
            id: viewportTmp
            Layout.fillWidth: true
            color: "grey"

            Text {
                anchors.fill: parent
                text: "WIP\nWaiting on Qt Vulkan"
                color: "darkred"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true 
                font.pixelSize: 42
             }
        }

        /*
        ViewportEditor {
            id: viewportEditor
            Layout.fillWidth: true 
            properties: properties
        }
        */

        SplitView {
            width: 200
            //anchors.fill: parent
            orientation: Qt.Vertical

            Outliner {
                id: outliner
                height: 200
                properties: properties
            }

            FieldEditor {
                id: fieldEditor
                height: 200
                properties: properties
                fieldModel: fields
            }

            LayerEditor {
                id: layerEditor
                properties: properties
            }

        }

    }

    TimeEditor {
        id: timeEditor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: cmdLineId.top
    }
 
    CommandLine {
        id: cmdLineId
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 25
        properties: properties
    }

    statusBar: StatusBar {
        RowLayout { BusyIndicator { implicitWidth: 10; implicitHeight: 10; running: false } Label { text: "Feather 0.1" } }
    }

    // this is for catching key presses on the main window
    Item {
        anchors.fill: parent
        focus: true

        Keys.onPressed: {
            console.log("key pressed")
            if((event.modifiers == Qt.ControlModifier)&&(event.key == Qt.Key_F)){
                console.log("F key hit")
                if(window.visibility == Window.FullScreen)
                    window.visibility = Window.Windowed
                else
                    window.visibility = Window.FullScreen
                event.accepted=true
            }
        }
    }

    Component.onCompleted: {
        //window.showFullScreen()        
    }

    //Material {}

}
