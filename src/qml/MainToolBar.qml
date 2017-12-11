/***********************************************************************
 *
 * Filename: MainToolBar.qml 
 *
 * Description: ToolBar for the main window. 
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

import QtQuick 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ToolBar {
    height: 32
    style: ToolBarStyle {}

    // ACTIONS

    // Open 
    Action {
        id: openAction
        text: "Open"
        tooltip: "Open Feather Model"
        onTriggered: {}
    }

    // Save 
    Action {
        id: saveAction
        text: "Save"
        tooltip: "Save Feather Model"
        onTriggered: {}
    }
 
    // Undo 
    Action {
        id: undoAction
        text: "Undo"
        tooltip: "Undo last action"
        onTriggered: {}
    }
 
    // Redo 
    Action {
        id: redoAction
        text: "Redo"
        tooltip: "Redo last action"
        onTriggered: {}
    }
 
    // Vertex Select 
    Action {
        id: vertexSelectAction
        text: "Vertex Selection"
        tooltip: "Select Vertex"
        onTriggered: {}
    }

    // Edge Select 
    Action {
        id: edgeSelectAction
        text: "Edge Selection"
        tooltip: "Select Edge"
        onTriggered: {}
    }

    // Face Select 
    Action {
        id: faceSelectAction
        text: "Face Selection"
        tooltip: "Select Face"
        onTriggered: {}
    }

    // Object Select 
    Action {
        id: objectSelectAction
        text: "Object Selection"
        tooltip: "Select Object"
        onTriggered: {}
    }

    // Properties
    Action {
        id: propertyAction
        text: "Properties"
        tooltip: "Open Properties window"
        onTriggered: {}
    }

    // Smooth Shading 
    Action {
        id: smoothShadeAction
        text: "Smooth Shading"
        tooltip: "Display models in smooth shading mode"
        onTriggered: {}
    }

    // Orthographic 
    Action {
        id: orthographicAction
        text: "Orthographic"
        tooltip: "Go into orthographic mode"
        onTriggered: {}
    }

    // Grid 
    Action {
        id: gridAction
        text: "Grid"
        tooltip: "Show grid"
        onTriggered: {}
    }

    // Grid 
    Action {
        id: axisAction
        text: "Axis"
        tooltip: "Show axis"
        onTriggered: {}
    }


    // LAYOUT

    RowLayout {
        spacing: 6
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/open.png"
            iconName: "open file"
            action: openAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/save.png"
            iconName: "save file"
            action: saveAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/undo.png"
            iconName: "undo action"
            action: undoAction
        }

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/redo.png"
            iconName: "redo action"
            action: redoAction
        }

    }

    RowLayout {
        spacing: 6
        anchors.centerIn: parent
        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/point_select.png"
            iconName: "point select"
             action: vertexSelectAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/edge_select.png"
            iconName: "edge select"
            action: edgeSelectAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/face_select.png"
            iconName: "face select"
            action: faceSelectAction
        }

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/object_select.png"
            iconName: "object select"
            action: objectSelectAction
        }

    }

    RowLayout {
        spacing: 6
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/props.png"
            iconName: "properties"
            action: propertyAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/smooth_shade.png"
            iconName: "smooth shading"
            action: smoothShadeAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/ortho.png"
            iconName: "orthographic view"
            action: orthographicAction
        } 

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/grid.png"
            iconName: "show grid"
            action: gridAction
        }

        ToolButton {
            width: 32; height: 32
            iconSource: "/usr/share/feather/ui/icons/axis.png"
            iconName: "show axis"
            action: axisAction
        }

    }


}
