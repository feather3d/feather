/***********************************************************************
 *
 * Filename: FileMenu.qml 
 *
 * Description: File specific menu items. 
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
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2 
import feather.command 1.0
import feather.scenegraph 1.0
import feather.tools 1.0
import "common"

Menu {
    id: fileMenu 
    title: "File"
    visible: true 

    //property Properties properties: Null

    style: FMenuStyle {} //mENUsTyle { properties: fileMenu.properties }


    //Tools { id: tools }

    FileDialog {
        id: openDialog
        title: "Open Scene"
        nameFilters: [ "Feather files ( *.feather )" ]
        onAccepted: {
            //openFeatherFilename.stringValue = tools.urlToString(openDialog.fileUrl)
            //openFeather.exec()       
            //SceneGraph.triggerUpdate()
        }
        onRejected: {}
        Component.onCompleted: visible=false
    }

    // ACTIONS

    // Load 
    Action {
        id: loadAction
        text: "Load"
        tooltip: "Load feather file"
        onTriggered: { /*openDialog.visible = true*/ }
    }

    // Save 
    Action {
        id: saveAction
        text: "Save"
        tooltip: "Save feather file"
        onTriggered: { /*openDialog.visible = true*/ }
    }

    // Save As 
    Action {
        id: saveAsAction
        text: "Save As"
        tooltip: "Save feather file"
        onTriggered: { /*openDialog.visible = true*/ }
    }

    // MENU


    // Load 
    MenuItem {
        action: loadAction
    }

    // Save 
    MenuItem {
        action: saveAction
    }

    // Save As 
    MenuItem {
        action: saveAsAction
    }

    MenuSeparator {}

}
