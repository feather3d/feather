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

import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import feather.editors 1.0
import feather.scenegraph 1.0
import feather.field 1.0

Rectangle {
    id: sgWindow
    color: "black" 
    
    property Properties properties: Null
    property alias fieldModel: sg_editor.connection

    signal statusChanged(int state, string msg)
 
    // Dialogs
    AddNodeDialog{ id: addNodeDialog; properties: sgWindow.properties }
    
    SceneGraphConnectionDialog { id: connectionDialog; properties: sgWindow.properties }

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
            var uid = SceneGraph.selected_node()
            SceneGraph.remove_node(uid)
            SceneGraph.nodesRemoved()
            SceneGraph.triggerUpdate()
        }
    }

    // Add Connection 
    Action {
        id: addConnection 
        tooltip: "connect the two selected node connections"
        onTriggered: {
            sg_editor.connectNodes()
            SceneGraph.triggerUpdate()
        }
    }

    // Delete Connection 
    Action {
        id: deleteConnection 
        tooltip: "disconnect the two selected node connections"
        onTriggered: {
            sg_editor.disconnectNodes()
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
                width: 32; height: 32
                iconSource: "/usr/local/feather/ui/icons/delete_node.png"
                action: deleteNode 
            }

            ToolButton {
                id: addConnectionButton
                width: 32; height: 32
                iconSource: "/usr/local/feather/ui/icons/add_connection.png"
                action: addConnection 
            }

            ToolButton {
                id: deleteConnectionButton
                width: 32; height: 32
                iconSource: "/usr/local/feather/ui/icons/delete_connection.png"
                action: deleteConnection 
            }

        }

    } 

    SceneGraphEditor {
        id: sg_editor
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 2
    }    

    //PopupMenu { id: connMenu; model: connModel }
    PopupMenu { id: connMenu; model: sgWindow.fieldModel }

    function openConnectionMenu() {
        connMenu.x = sg_editor.clickX
        connMenu.y = sg_editor.clickY
        console.log(sg_editor.clickX + " " + sg_editor.clickY)
        connMenu.resize()
        connMenu.visible = true
    }

    function nodeSelection(type,uid,nid) {
        // This is needed to emit to the nodeSelected signal to the other widgets so they can update 
        SceneGraph.select_node(type,uid,nid);
      }

    function connectionButtonPressed(button,uid,nid,fid) {
        //sg_editor.connectionMousePressed(button,uid,nid,fid);
        //SceneGraph.select_node(0,uid,nid,fid);
        connectionDialog.visible
    }

    function connectionButtonReleased(button,uid,nid,fid) {
        sg_editor.connectionMouseReleased(button,uid,nid,fid);
    }

    function connectionButtonClicked(button,uid,nid,fid) {
        sg_editor.connectionMouseClicked(button,uid,nid,fid);
    }

    function selectNode() {
        sg_editor.updateNodes()
    }
    
    function add_node(name,nid) {
        console.log("Add node: " + name + " nid: " + nid)
        var uid = SceneGraph.add_node(nid,name)
        //console.log("added node as uid: " + uid)
        var suid = SceneGraph.selected_node()
        var p = SceneGraph.connect_nodes(suid,202,uid,201)
        //console.log("connection status was: " + p)
        SceneGraph.triggerUpdate()
    }

    function connectionClicked(button,connection,x,y,uid,nid) {
        // close the previous connection dialog if it's still open
        connectionDialog.visible = false
        connectionDialog.x = x
        connectionDialog.y = y
        connectionDialog.uid = uid
        connectionDialog.nid = nid
        connectionDialog.connection = connection
        connectionDialog.visible = true
    }

    function connectionSelection(uid,nid,fid,connection) {
        //console.log("connectionSelection uid:",uid," nid:",nid," fid:",fid," connection:",connection)
        if(connection == Field.In) {
            sg_editor.setConnectionTarget(uid,fid)
            SceneGraph.triggerUpdate()
        } else {
            sg_editor.setConnectionSource(uid,fid)
            SceneGraph.triggerUpdate()
        }
    }

    function statusTriggered(state,msg){
        console.log("STATUS TRIGGERED: state:",state," msg:",msg)
    }

    Component.onCompleted: {
        sg_editor.openConnMenu.connect(openConnectionMenu)
        sg_editor.nodeSelection.connect(nodeSelection)
        //connMenu.connectionButtonPressed.connect(connectionButtonPressed)
        //connMenu.connectionButtonReleased.connect(connectionButtonReleased)
        //connMenu.connectionButtonClicked.connect(connectionButtonClicked)
        SceneGraph.nodeSelected.connect(selectNode)
        SceneGraph.updateGraph.connect(sg_editor.update_sg)
        SceneGraph.cleared.connect(sg_editor.update_sg)
        addNodeDialog.addNode.connect(add_node)
        sg_editor.connectorClicked.connect(connectionClicked)
        connectionDialog.fieldSelected.connect(connectionSelection)
        // status changed
        sg_editor.statusChanged.connect(statusTriggered)
    }

}
