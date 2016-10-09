/***********************************************************************
 *
 * Filename: MorphEditor.qml 
 *
 * Description: Container for node editor dialogs.
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
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import feather.scenegraph 1.0
import feather.field 1.0

Dialog {
    id: weightEditor 
    title: "Morph Editor"
    visible: false 
    width: 400
    height: 400
    property Properties properties: null
    standardButtons: StandardButton.Ok

    ListModel { id: weightModel }

    ListView {
        id: view
        spacing: 2
        anchors.fill: parent
        focus: true
        model: weightModel

        delegate: WeightEditorValue {
            anchors.left: parent.left
            anchors.right: parent.right
            name: model.name
            uid: model.uid
            fid: model.fid
            weightId: model.weightId 
        }  
    }        

    function load_node() {
        var cuid = SceneGraph.selected_node()
        title = "Morph Editor - [" + SceneGraph.node_name(cuid) + "]" 
        weightModel.clear()
        if(SceneGraph.node_id(cuid) == 426) {
            var meshes = SceneGraph.connected_uids(cuid,2)
            console.log("connected mesh count = " + meshes.length)
            for(var i=0; i < meshes.length; i++){
                var name = SceneGraph.node_name(meshes[i])
                weightModel.insert(i,{"name": name,"uid": cuid,"fid": 3,"weightId": i})
            }
        }
    }

    onVisibleChanged: {
        load_node()
    }

}
