/***********************************************************************
 *
 * Filename: SubdivMenu.qml 
 *
 * Description: Container for all menu items related to the subdivisions.
 *
 * Copyright (C) 2016 Richard Layman, rlayman2000@yahoo.com 
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
import QtQuick.Dialogs 1.0 
import feather.scenegraph 1.0
import feather.node 1.0
import feather.command 1.0

Menu {
    id: subdivMenu
    title: "Subdiv"
    visible: true

    property Properties properties: Null

    style: MenuStyle { properties: subdivMenu.properties }

    // COMMANDS
    /*
    Command {
        id: importObj
        name: "import_obj"
        parameters: [
            Parameter { 
                name: "filename"
                type: Parameter.String
                stringValue: "test"
            },
            Parameter { 
                name: "selection"
                type: Parameter.Bool
                boolValue: true
            }
        ]
    }
    */

    // ACTIONS

    // Convert Selected 
    Action {
        id: convertToSubdivAction
        text: "Convert Selected"
        tooltip: "convert selected polygon shape to a subdiv"
        onTriggered: {
            // can only convert shapes connected to subdivs
            var tuid = SceneGraph.selected_node()
            var ttype = SceneGraph.node_id(tuid)

            // if cube
            if(ttype == 320){
                console.log("found source shape")
                var uids = SceneGraph.connected_uids(tuid,1)
                console.log(" source connection count=",uids.length)
                for(var i=0; i < uids.length; i++){
                    console.log("looking at uid:",uids[i])
                    // if the connected node is a polygon, add a subdiv between the two nodes
                    var type = SceneGraph.node_id(uids[i])
                    // for now well only convert cubes and meshes
                    if(type == 322 || type == 324){
                        console.log("found cube or mesh input node")
                        var source = uids[i]
                        // remove mesh connection between the cube and shape
                        if(type == 322)
                            SceneGraph.disconnect_nodes(source,4,tuid,1)
                        else if(type == 324)
                            SceneGraph.disconnect_nodes(source,2,tuid,1)
                        // create the new node name
                        var subdivname = SceneGraph.node_name(source) + "_subdiv"
                        // create subdiv node and connect between the two
                        var subdiv = SceneGraph.add_node(323,subdivname)
                        if(type == 322)
                            SceneGraph.connect_nodes(source,4,subdiv,1)
                        else if(type == 324)
                            SceneGraph.connect_nodes(source,2,subdiv,1)
                        SceneGraph.connect_nodes(subdiv,5,tuid,1)
                        // update sg
                        SceneGraph.nodeUpdateDrawItems(subdiv)
                        SceneGraph.triggerUpdate()
                        SceneGraph.nodeUpdateDrawItems(tuid)
                        SceneGraph.triggerUpdate()
                    }
                } 
            }
        }
    }

    // Convert All 
    Action {
        id: convertAllSubdivAction
        text: "Convert All"
        tooltip: "convert all polygon shapes to subdiv"
        onTriggered: {

            // get all shape nodes
            var shapes = SceneGraph.get_nodes_by_id(320)
            console.log("found ",shapes.length," shapes")            
            // convert each shape
            for ( var i=0; i < shapes.length; i++ ) {

                var tuid = shapes[i] 
            
                console.log("found source shape")
                var uids = SceneGraph.connected_uids(tuid,1)
                console.log(" source connection count=",uids.length)
                for(var j=0; j < uids.length; j++){
                    console.log("looking at uid:",uids[j])
                    // if the connected node is a polygon, add a subdiv between the two nodes
                    var type = SceneGraph.node_id(uids[j])
                    // for now well only convert cubes and meshes
                    if(type == 322 || type == 324){
                        console.log("found cube or mesh input node")
                        var source = uids[j]
                        // remove mesh connection between the cube and shape
                        if(type == 322)
                            SceneGraph.disconnect_nodes(source,4,tuid,1)
                        else if(type == 324)
                            SceneGraph.disconnect_nodes(source,2,tuid,1)
                        // create the new node name
                        var subdivname = SceneGraph.node_name(source) + "_subdiv"
                        // create subdiv node and connect between the two
                        var subdiv = SceneGraph.add_node(323,subdivname)
                        if(type == 322)
                            SceneGraph.connect_nodes(source,4,subdiv,1)
                        else if(type == 324)
                            SceneGraph.connect_nodes(source,2,subdiv,1)
                        SceneGraph.connect_nodes(subdiv,5,tuid,1)
                        // update sg
                        SceneGraph.nodeUpdateDrawItems(subdiv)
                        SceneGraph.triggerUpdate()
                        SceneGraph.nodeUpdateDrawItems(tuid)
                        SceneGraph.triggerUpdate()
                    }
                } 
            }
        }
    }


    // Convert Selected 
    MenuItem {
        action: convertToSubdivAction
    }

    // Convert All 
    MenuItem {
        action: convertAllSubdivAction
    }

}
