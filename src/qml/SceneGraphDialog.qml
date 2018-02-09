/***********************************************************************
 *
 * Filename: SceneGraphDialog.qml 
 *
 * Description: Main window for the scenegraph editor 
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
import feather.scenegraph 1.0

Window {
    id: sgEditor 
    title: "SceneGraph Editor"
    visible: true
    flags: Qt.Tool
    width: 400
    height: 500

    property Properties properties: Null

    SceneGraphEditor {
        id: sg
        anchors.fill: parent
        scenegraph: SceneGraph
        properties: sgEditor.properties
    }
}
