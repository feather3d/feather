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

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import feather.scenegraph 1.0
import feather.field 1.0
import "common"

ApplicationWindow {
    id: mainwindow
    width: 1900
    height: 1020 
    visible: true
    title: "Feather 0.2"
    color: theme.windowBorderNormalColor 


    FTheme { id: theme }

    Properties { id: prop }

    menuBar: MenuBar {
        id: mainMenu

        style: FMenuBarStyle {} 

        FileMenu { id: fileMenu; properties: prop }
    } 

    SceneGraphEditor {
        id: sgEditor
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: fieldEditor.left 
        anchors.bottom: timeEditor.top
        properties: prop
    }

    FieldEditor {
        id: fieldEditor
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: timeEditor.top
        properties: prop
        fieldModel: FieldModel{} 
    }

    TimeEditor {
        id: timeEditor
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    /*
    FSplitView {
        id: view
        anchors.fill: parent

        TestWindow { id: window1 }
        TestWindow { id: window2 }
    }
    */
}
