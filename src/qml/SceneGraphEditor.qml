/***********************************************************************
 *
 * Filename: SceneGraphEditor.qml 
 *
 * Description: Holds the actual C++ scenegraph widget. 
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
import QtQuick.Layouts 1.1
import feather.editors 1.0
import feather.scenegraph 1.0
import feather.field 1.0

Rectangle {
    id: sgWindow
    color: "black" 
    
    property SceneGraph scenegraph: Null
    property Properties properties: Null

    // Dialogs
    AddNodeDialog{ id: addNodeDialog; properties: sgWindow.properties }

    // Actions

    // Add Node 
    Action {
        id: addNode
        tooltip: "Add a node to the scenegraph"
        onTriggered: { addNodeDialog.visible=true }
    }

    // Delete Node 
    Action {
        id: deleteNode
        tooltip: "Remove selected node from scenegraph"
        onTriggered: {
            var uid = scenegraph.selected_node()
            scenegraph.remove_node(uid)
            scenegraph.nodesRemoved()
            scenegraph.triggerUpdate()
        }
    }


    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 36
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
            anchors.left: parent.left
             
            ToolButton {
                id: addNodeButton
                width: 32; height: 32
                iconSource: "/usr/local/feather/ui/icons/add_node.png"
                action: addNode 
            }

            ToolButton {
                id: deleteNodeButton
                width: 32; height: 23
                iconSource: "/usr/local/feather/ui/icons/delete_node.png"
                action: deleteNode 
            }

        }

    } 

    FieldModel { id: connModel }

    SceneGraphEditor {
        id: sg_editor
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 2
        connection: connModel
    }    

    PopupMenu { id: connMenu; model: connModel }
    function openConnectionMenu() {
        connMenu.x = sg_editor.clickX
        connMenu.y = sg_editor.clickY
        console.log(sg_editor.clickX + " " + sg_editor.clickY)
        connMenu.resize()
        connMenu.visible = true
    }

    function nodeSelection(type,uid,nid) {
        scenegraph.select_node(type,uid,nid);
    }

    function connectionButtonPressed(button,uid,nid,fid) {
        sg_editor.connectionMousePressed(button,uid,nid,fid);
        scenegraph.select_node(0,uid,nid,fid);
    }

    function connectionButtonReleased(button,uid,nid,fid) {
        sg_editor.connectionMouseReleased(button,uid,nid,fid);
    }

    function connectionButtonClicked(button,uid,nid,fid) {
        sg_editor.connectionMouseClicked(button,uid,nid,fid);
    }

    function setSelection(type,uid,nid,fid) {
        console.log("sg triggered");
        console.log("sg editor triggered for uid " + uid + " nid " + nid + " fid " + fid);
    }

    function add_node(name,type,nid) {
        console.log("Add node: " + name + " type: " + type + " nid: " + nid)
        var uid = scenegraph.add_node(type,nid,name)
        //console.log("added node as uid: " + uid)
        var p = scenegraph.connect_nodes(0,2,uid,1)
        //console.log("connection status was: " + p)
        scenegraph.triggerUpdate()
    }

    Component.onCompleted: {
        sg_editor.update_sg()
        sg_editor.openConnMenu.connect(openConnectionMenu)
        sg_editor.nodeSelection.connect(nodeSelection)
        connMenu.connectionButtonPressed.connect(connectionButtonPressed)
        connMenu.connectionButtonReleased.connect(connectionButtonReleased)
        connMenu.connectionButtonClicked.connect(connectionButtonClicked)
        scenegraph.nodeSelected.connect(setSelection)
        scenegraph.updateGraph.connect(sg_editor.update_sg)
        scenegraph.cleared.connect(sg_editor.update_sg)
        addNodeDialog.addNode.connect(add_node)
    }

}
