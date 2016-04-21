/***********************************************************************
 *
 * Filename: ViewportEditor.qml 
 *
 * Description: Holds viewports and other items to interact with it. 
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

import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.2
import feather.scenegraph 1.0
import feather.viewport 1.0

Item {
//Window {
    id: editor 
    //title: "Viewport Editor"
    //visible: true
    //flags: Qt.SubWindow
    //flags: Qt.Tool
    //width: 600
    //height: 600
    property Properties properties: Null

    // Show Viewport Components

    Action {
        id: showAxisAction
        text: "Show Axis"
        tooltip: "show the axis"
        checkable: true
        checked: true
        onTriggered: { (checked) ? viewport.showAxis=true : viewport.showAxis=false }
    }

    Action {
        id: showGridAction
        text: "Show Grid"
        tooltip: "show the grid"
        checkable: true
        checked: true
        onTriggered: { (checked) ? viewport.showGrid=true : viewport.showGrid=false }
    }


    // TODO - saving this for selections with the idea that the toolbutton's state will represenct the state of the selected mesh
    /*
    // Show Mesh Components

    Action {
        id: showShadedAction
        text: "Show Shaded"
        tooltip: "show the grid"
        checkable: true
        checked: true
        onTriggered: { (checked) ? viewport.showShaded=true : viewport.showShaded=false }
    }

    Action {
        id: showWireframeAction
        text: "Show Wireframe"
        tooltip: "show the grid"
        checkable: true
        checked: false 
        onTriggered: { (checked) ? viewport.showWireframe=true : viewport.showWireframe=false }
    }

    Action {
        id: showPointsGridAction
        text: "Show Points"
        tooltip: "show the grid"
        checkable: true
        checked: false 
        onTriggered: { (checked) ? viewport.showPoints=true : viewport.showPoints=false }
    }

    */

    ExclusiveGroup {
        id: selectionGroup

        Action {
            id: vertexSelectAction
            text: "Point Selection"
            tooltip: "select points"
            checkable: true
            onTriggered: { }
        }

        Action {
            id: edgeSelectAction
            text: "Edge Selection"
            tooltip: "select edges"
            checkable: true
            onTriggered: { }
        }

        Action {
            id: faceSelectAction
            text: "Face Selection"
            tooltip: "select faces"
            checkable: true
            onTriggered: { }
        }

        Action {
            id: objectSelectAction
            text: "Object Selection"
            tooltip: "select objects"
            checkable: true
            onTriggered: { }
        }
 
    }

    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 72
        style: ToolBarStyle {
            background: Rectangle {
                color: properties.getColor("toolBarBg")
                border.color: "black"
                border.width: 1
            }
        }

        RowLayout {
            spacing: 6
            anchors.verticalCenter: parent.verticalCenter
            anchors.fill: parent
 
            GroupBox {
                title: "Selection Mode"
 
                Row {
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter
                    //anchors.left: parent.left
                    anchors.fill: parent      
                    ToolButton {
                        id: vertexSelectBtn
                        width: 32; height: 32
                        checkable: true
                        iconSource: "/usr/local/feather/ui/icons/point_select.png"
                        action: vertexSelectAction 
                    }
            
                    ToolButton {
                        id: edgeSelectBtn
                        width: 32; height: 32
                        checkable: true
                        iconSource: "/usr/local/feather/ui/icons/edge_select.png"
                        action: edgeSelectAction 
                    }
            
                    ToolButton {
                        id: faceSelectBtn
                        width: 32; height: 32
                        checkable: true
                        iconSource: "/usr/local/feather/ui/icons/face_select.png"
                        action: faceSelectAction 
                    }

                    ToolButton {
                        id: objectSelectBtn
                        width: 32; height: 32
                        checkable: true
                        iconSource: "/usr/local/feather/ui/icons/object_select.png"
                        action: objectSelectAction 
                    }
                }
            }

            Item { Layout.fillWidth: true }

            GroupBox {
                title: "Viewport"
 
                Row {
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter
                    //anchors.left: parent.left
                    anchors.fill: parent

                    ToolButton {
                        id: showAxisBtn
                        width: 32; height: 32
                        checkable: true
                        iconSource: "/usr/local/feather/ui/icons/axis.png"
                        action: showAxisAction 
                    }
            
                    ToolButton {
                        id: showGridBtn
                        width: 32; height: 32
                        checkable: true
                        iconSource: "/usr/local/feather/ui/icons/grid.png"
                        action: showGridAction 
                    }
                }
            }



        }

    } 

    /*
    Viewport3D2 { 
        id: viewport
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    */
        /*
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
        }
        */
    //}

    //function updateGL() { viewport.updateGL() }

    // vulkan viewport
    Viewport { 
        id: viewport
        //anchors.top: toolBar.bottom
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.bottom: parent.bottom
    } 

    Component.onCompleted: { 
        //editor.update(); updateGL()
    }

}
