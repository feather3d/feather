/***********************************************************************
 *
 * Filename: NodeMenuItem.qml 
 *
 * Description: Base for loaded menu items for nodes. 
 *
 * Copyright (C) 2018 Richard Layman, rlayman2000@yahoo.com 
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
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2 
import feather.command 1.0
import feather.scenegraph 1.0
import feather.tools 1.0
import feather.node 1.0


MenuItem {
    id: item
    text: name

    property int nid: 0
    property int ntype: 0
    property string name: ""

    function add_node(name,nid) {
        console.log("Add node: " + name + " nid: " + nid)
        var uid = SceneGraph.add_node(nid,name)
        //console.log("added node as uid: " + uid)
        var suid = SceneGraph.selected_node()
        var p = SceneGraph.connect_nodes(suid,202,uid,201)
        //console.log("connection status was: " + p)
        SceneGraph.triggerUpdate()
    }

    onTriggered: { add_node(name,nid) }
}
