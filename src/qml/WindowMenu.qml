/***********************************************************************
 *
 * Filename: WindowMenu.qml 
 *
 * Description: Contains menu items for various dialog windows. 
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
import QtQuick.Controls 1.2
import feather.scenegraph 1.0

Menu {
    id: windowMenu 
    title: "Window"
    visible: true

    property Properties properties: Null

    style: MenuStyle { properties: windowMenu.properties }

    MorphEditor { id: morphEditor }

    // ACTIONS


    // Outliner 
    Action {
        id: outlinerAction
        text: "Outliner"
        tooltip: "Open Outliner window"
        onTriggered: {}
    }

    // Material 
    Action {
        id: materialAction
        text: "Material"
        tooltip: "Open Material window"
        onTriggered: {}
    }

    // Material 
    Action {
        id: morphEditorAction
        text: "Morph Editor"
        tooltip: "Open Morph Editor"
        onTriggered: { morphEditor.visible=true}
    }


    // MENU


    // Outliner 
    MenuItem {
        action: outlinerAction
    }

    // Morph Editor 
    MenuItem {
        action: morphEditorAction 
    }

    // Material 
    MenuItem {
        action: materialAction
    }

    MenuSeparator {}

}
