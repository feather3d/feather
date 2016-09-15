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

import QtQuick 2.5
import QtQuick.Window 2.2
import feather.scenegraph 1.0
import feather.field 1.0

Window {
    id: editor
    //title: "SceneGraph Editor"
    visible: false 
    flags: Qt.Popup
    width: 100
    height: 400
    property Properties properties: Null
    property int uid: 0 
    property int nid: 0
    property int connection: Field.In
    property int fieldHeight: 15

    color: "grey"

    signal fieldSelected(int uid, int nid, int fid, int connection)         
    NodeFields { id: fields }

    onVisibleChanged: {
        console.log("scenegraph dialog visible changed for uid:",uid,", nid:",nid,", connection:",connection)
        fields.addFields(uid,nid,connection)
        editor.height = (view.count * fieldHeight) + (view.count * 2) + 2
    }

    ListView {
        id: view
        spacing: 2
        anchors.fill: parent
        anchors.margins: 2

        Component {
            id: fieldDelegate
            Rectangle {
                id: fieldbox
                height: fieldHeight 
                width: parent.width
                color: properties.getFieldTypeColor(type)
                opacity: (connected) ? 0.5 : 1.0 
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: name
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onPressed: {
                        //console.log("field ",fid," of uid ",uid," clicked")
                        //if(!connected || connection == Field.Out)
                        fieldSelected(uid,nid,fid,connection)
                        editor.visible = false
                    }
              
                    onEntered: {
                        color = "green"
                    }

                    onExited: {
                        console.log("fid:",fid," type:",type)
                        color = properties.getFieldTypeColor(type)
                    }
                }
            }
        }

        model: fields
        delegate: fieldDelegate 
        //delegate: FieldEditorValue { properties: fieldEditor.properties; width: parent.width; uidKey: uid; nidKey: nid; fidKey: fid; fieldType: type; label: name }
        focus: true
    }
}
