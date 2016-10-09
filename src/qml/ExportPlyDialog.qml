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
import QtQuick.Dialogs 1.2 
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import feather.scenegraph 1.0
import feather.command 1.0
import feather.tools 1.0

Dialog {
    id: dialog
    title: "Export Ply Options"
    width: 400
    height: 240
    modality: Qt.ApplicationModal
    visible: false
    //color: properties.getColor("windowBg")
    standardButtons: StandardButton.Cancel | StandardButton.Ok

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
            },
            Parameter { 
                id: exportAnimation
                name: "animation"
                type: Parameter.Bool
                boolValue: false 
            },
            Parameter { 
                id: exportStartFrame
                name: "sframe"
                type: Parameter.Int
                intValue: 0 
            },
            Parameter { 
                id: exportEndFrame
                name: "eframe"
                type: Parameter.Int
                intValue: 24 
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

        // Selection
        GroupBox {
            title: "Export Options"

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

        IntValidator { id: intValidator }

        // Animation
        GroupBox {
            title: "Animation Options"

            Row {
                spacing: 4

                RadioButton {
                    text: "Animation"
                    checked: false 
                    //exclusiveGroup: exportedMeshesGroup
                    onClicked: { (checked) ? exportAnimation.boolValue = true : exportAnimation.boolValue = false }
                 }

                TextField {
                    text: "0"
                    width: 80 
                    height: 30
                    validator: intValidator
                    onEditingFinished: { exportStartFrame.intValue = text }
                }

                TextField {
                    text: "24"
                    width: 80 
                    height: 30
                    validator: intValidator
                    onEditingFinished: { exportEndFrame.intValue = text }
                }
            }
        }

        // Path
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


        /*
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
        */
    }

    onAccepted: {
         console.log("animation:",exportAnimation.boolValue," sframe:",exportStartFrame.intValue," eframe:",exportEndFrame.intValue) 
         if(pathSet){
            console.log("EXPORTING PLY FILES")
            exportPly.exec()
            SceneGraph.triggerUpdate()
            dialog.visible = false
        }
    }

    onRejected: { dialog.visible = false}
}
