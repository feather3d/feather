/***********************************************************************
 *
 * Filename: MainMenu.qml 
 *
 * Description: Top menu for the main window.
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
import QtQuick.Controls.Styles 1.4

MenuBar {
    id: menu
    property Properties properties: Null

    style: MenuBarStyle {
        background: Rectangle {
            color: properties.getColor("menuBarBg")
            border.color: "black"
            border.width: 1
        }
    }
 
    FileMenu{ properties: menu.properties }

    EditMenu{ properties: menu.properties }

    AddMenu{ properties: menu.properties }

    SubdivMenu{ properties: menu.properties }

    ViewMenu{ properties: menu.properties }

    SelectMenu{ properties: menu.properties }

    ToolsMenu{ properties: menu.properties }

    WindowMenu{ properties: menu.properties }

    HelpMenu{ properties: menu.properties }
}
