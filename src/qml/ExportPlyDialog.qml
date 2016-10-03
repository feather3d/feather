/***********************************************************************
 *
 * Filename: ExportPlyDialog.qml 
 *
 * Description: Dialog to add a new node to the scenegraph.
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
import QtQuick.Dialogs 1.0 
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import feather.scenegraph 1.0
import feather.command 1.0
import feather.tools 1.0

Window {
    id: dialog
    title: "Export Ply Options"
    width: 400
    height: 160
    modality: Qt.WindowModal
    flags: Qt.Dialog
    visible: false
    x: (Screen.desktopAvailableWidth - width)/2
    y: (Screen.desktopAvailableHeight - height)/2
    color: properties.getColor("windowBg")

    property Properties properties: Null
    property bool pathSet: false
 

    Command {
        id: exportPly 
        name: "export_ply"
        parameters: [
            Parameter { 
                id: exportPlyFilename
                name: "path"
                type: Parameter.String
                stringValue: ""
            },
            Parameter {
                id: exportSelection 
                name: "selection"
                type: Parameter.Bool
                boolValue: true
            }
        ]
    }

    FileDialog {
        id: pathDialog
        title: "Choose Ply Target Folder"
        selectExisting: true 
        selectFolder: true 
        selectMultiple: false
        //nameFilters: [ "Ply format ( *.ply)" ]
        onAccepted: {
            pathField.placeholderText = tools.urlToString(pathDialog.folder) + "/" 
            exportPlyFilename.stringValue = pathField.placeholderText
            pathSet = true
        }
        onRejected: { pathField.placeholderText = ""; pathSet = false} 
    }

    Column {
        spacing: 4
        anchors.fill: parent
        anchors.margins: 4

        GroupBox {
            title: "Exported Meshes"

            Row {
                spacing: 4

                ExclusiveGroup { id: exportedMeshesGroup }
                RadioButton {
                    text: "Selected"
                    checked: true 
                    exclusiveGroup: exportedMeshesGroup
                    onClicked: { exportSelection.boolValue = true }
                 }

                RadioButton {
                    text: "All"
                    exclusiveGroup: exportedMeshesGroup
                    onClicked: { exportSelection.boolValue = false }
                }
            }
        }

        GroupBox {
            title: "Path"

            Row {
                spacing: 4

                TextField {
                    id: pathField 
                    width: 280
                    height: 30
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    readOnly: true
                    enabled: false 
                    placeholderText: "Select Path"
                    style: TextFieldStyle {
                        background: Rectangle {
                            color: (pathSet) ? properties.getColor("textBg") : "red"
                            border.color: "black"
                            border.width: 1
                            radius: 2
                        }

                        textColor: properties.getColor("text")
                    }
                }
 
                PushButton {
                    id: pathButton
                    width: 100 
                    height: 30
                    text: "Select"
                    properties: dialog.properties 

                    onClicked: { pathDialog.visible = true }
                }
            }
        }


        Row {
            spacing: 4
            anchors.horizontalCenter: parent.horizontalCenter

            PushButton {
                id: cancelButton
                width: 100 
                height: 30
                text: "Cancel"
                properties: dialog.properties 
            }

            PushButton {
                id: exportButton
                width: 100 
                height: 30
                text: "Export"
                properties: dialog.properties 
            }
        }
    }

    function onCancel() {
        dialog.visible = false
    }

    function onExport() {
        if(pathSet){
            console.log("EXPORTINT PLY FILES")
            exportPly.exec()       
            SceneGraph.triggerUpdate()
            dialog.visible = false
        }
    }

    Component.onCompleted: {
        cancelButton.clicked.connect(onCancel)
        exportButton.clicked.connect(onExport)
    }
}
