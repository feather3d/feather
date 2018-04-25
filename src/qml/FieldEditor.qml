/***********************************************************************
 *
 * Filename: FieldEditor.qml 
 *
 * Description: Container for node editor dialogs.
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
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import feather.scenegraph 1.0
import feather.field 1.0
import "common"

Item {
//Window {
    id: fieldEditor 
    //title: "FieldEditor"
    //visible: true
    //flags: Qt.Tool
    width: 200
    height: 500
    property Properties properties: Null
    property alias fieldModel: view.model

    Rectangle {
        id: nodeFrame
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height/2
        anchors.margins: 1
        border.color: "black"
        border.width: 1
        color: properties.getColor("windowBg")
        //focus: true

        // Node Title
        Rectangle {
            id: nodeTitle
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20
            border.color: "black"
            border.width: 1
            color: properties.getColor("windowBg")

            Text {
                id: nodeLabel
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: false 
                font.pixelSize: 12
                color: "black"
            }

            TextField {
                id: nodeLabelEdit 
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 0
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                height: 20
                font.bold: false 
                font.pixelSize: 12
                visible: false
                placeholderText: "Enter Node Name"
                focus: true

                style: TextFieldStyle {
                    background: Rectangle {
                        radius: 2
                        border.width: 1
                        border.color: "black"
                        color: "darkgrey"
                    }
                }

                onAccepted: {
                    var uid = SceneGraph.selected_node()
                    SceneGraph.set_node_name(uid,nodeLabelEdit.text)
                    SceneGraph.triggerUpdate()
                    nodeLabel.focus = true 
                    visible = false
                    nodeLabel.text = SceneGraph.node_name(uid)
                    nodeLabel.visible = true
                    console.log("EDITING FINISHED")
                    update()
                }
            } 

            MouseArea {
                id: nodeTitleMouseArea
                anchors.fill: parent
                hoverEnabled: true 
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onDoubleClicked: {
                    nodeLabel.visible=false
                    nodeLabelEdit.visible=true
                    nodeLabelEdit.focus=true
                }
            }
        }

        ListView {
            id: view
            spacing: 2
            anchors.top: nodeTitle.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 1
            focus: true

            delegate: FieldEditorValue {
                properties: fieldEditor.properties
                width: parent.width
                uidKey: uid
                nidKey: nid
                fidKey: fid
                fieldType: type
                label: name
                // TODO - this is a temp fix to get rid of unwanted types showing up in the editor
                visible: (type==Field.Mesh || type==Field.Matrix3x3 || type==Field.Matrix4x4 || type==Field.Node || type==Field.NodeArray || type==Field.RealArray || type==Field.IntArray) ? false : true
                height: (type==Field.Mesh || type==Field.Matrix3x3 || type==Field.Matrix4x4 || type==Field.Node || type==Field.NodeArray || type==Field.RealArray || type==Field.IntArray) ? 0 : 15 
            }
        }
    }
 
    Rectangle {
        id: baseFrame
        anchors.top: nodeFrame.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 1
        border.color: "black"
        border.width: 1

        // Node Base Title
        Rectangle {
            id: nodeBaseTitle
            anchors.top: parent.top 
            anchors.left: parent.left
            anchors.right: parent.right
            height: 15
            border.color: "black"
            border.width: 1
            //color: "slategrey"
 
            Text {
                id: nodeBaseLabel
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: false 
                font.pixelSize: 12
                color: "black"
                text: "base"
            }
        }

    }
        
 
    function setSelection(type,uid,nid,fid) {
        console.log("fed triggered");
        console.log("field editor triggered for uid " + uid + " nid " + nid + " fid " + fid);
        fieldModel.addField(uid,nid,fid,0,false)
        fieldModel.layoutChanged()
    }

    function nodeSelect() {
        var uid = SceneGraph.selected_node()
        var nid = SceneGraph.node_id(uid)
        nodeLabel.text = SceneGraph.node_name(uid)
        fieldModel.addFields(uid,nid,Field.In)
    }

    function updateColor() {
        console.log("update node color")
        nodeFrame.color = properties.getColor("windowBg")        
        baseFrame.color = properties.getColor("windowBg")        
        nodeTitle.color = properties.getColor("labelBg")        
        nodeBaseTitle.color = properties.getColor("labelBg")        
        //fieldEditor.color = properties.getColor("windowBg")

    }

    Component.onCompleted: {
        // This was removed to fix seq fault during outliner selection
        // It needs to be enabled again once the outliner is working
        //SceneGraph.nodeSelected.connect(setSelection)        
        SceneGraph.nodeSelected.connect(nodeSelect)
        fieldEditor.properties.colorsChanged.connect(updateColor)
        updateColor()
    }    
}
