/***********************************************************************
 *
 * Filename: FileMenu.qml 
 *
 * Description: File specific menu items. 
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
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.0 
import feather.command 1.0
import feather.scenegraph 1.0
import feather.tools 1.0


Menu {
    id: fileMenu 
    title: "File"
    visible: true

    property Properties properties: Null

    style: MenuStyle { properties: fileMenu.properties }


    Tools { id: tools }

    FileDialog {
        id: openDialog
        title: "Open Scene"
        nameFilters: [ "Feather files ( *.feather )" ]
        onAccepted: {
            openFeatherFilename.stringValue = tools.urlToString(openDialog.fileUrl)
            openFeather.exec()       
            SceneGraph.triggerUpdate()
        }
        onRejected: {} 
    }

    FileDialog {
        id: saveDialog
        title: "Save Scene"
        selectExisting: false
        nameFilters: [ "Feather files ( *.feather )" ]
        onAccepted: {
            saveFeatherFilename.stringValue = tools.urlToString(saveDialog.fileUrl)
            saveFeather.exec()       
            SceneGraph.triggerUpdate()
        }
        onRejected: {} 
    }

    FileDialog {
        id: importDialog
        title: "Import Obj"
        nameFilters: [ "Obj files ( *.obj)" ]
        onAccepted: {
            importObjFilename.stringValue = tools.urlToString(importDialog.fileUrl)
            importObj.exec()       
            SceneGraph.triggerUpdate()
        }
        onRejected: {} 
    }

    ExportPlyDialog {
        id: exportDialog
        properties: fileMenu.properties
    }

    FileDialog {
        id: exportCameraDataDialog
        title: "Export Camera Data"
        selectExisting: false 
        //selectFolder: true 
        //selectMultiple: false
        nameFilters: [ "Camera format ( *.cam)" ]
        onAccepted: {
            exportCameraDataFilename.stringValue = tools.urlToString(exportCameraDataDialog.fileUrl) 
            // TODO - need to make it so that any camera can be exported instead of just uid 2
            exportCameraDataUid.intValue = 2 
            exportCameraData.exec()       
            SceneGraph.triggerUpdate()
        }
        onRejected: {} 
    }

    PropertiesDialog {
        id: propDialog
        visible: false
        properties: fileMenu.properties
    }

    // COMMANDS
    Command {
        id: openFeather
        name: "open_feather"
        parameters: [
            Parameter { 
                id: openFeatherFilename
                name: "filename"
                type: Parameter.String
                stringValue: ""
            }
        ]
    }

    Command {
        id: saveFeather 
        name: "save_feather"
        parameters: [
            Parameter { 
                id: saveFeatherFilename
                name: "filename"
                type: Parameter.String
                stringValue: ""
            }
        ]
    }

    Command {
        id: importObj
        name: "import_obj"
        parameters: [
            Parameter { 
                id: importObjFilename
                name: "filename"
                type: Parameter.String
                stringValue: ""
            },
            Parameter { 
                name: "selection"
                type: Parameter.Bool
                boolValue: true
            }
        ]
    }

    Command {
        id: exportCameraData
        name: "export_camera_data"
        parameters: [
            Parameter { 
                id: exportCameraDataFilename
                name: "path"
                type: Parameter.String
                stringValue: ""
            },
            Parameter {
                id: exportCameraDataUid
                name: "uid"
                type: Parameter.Int
                intValue: 2 
            }
        ]
    }

    // ACTIONS

    // New 
    Action {
        id: newAction
        text: "New"
        tooltip: "Clear the scene"
        onTriggered: { SceneGraph.clear() }
    }

    // Open
    Action {
        id: openAction
        text: "Open"
        tooltip: "Open scene"
        onTriggered: { openDialog.visible = true }
    }

    // Save 
    Action {
        id: saveAction
        text: "Save"
        tooltip: "Save scene"
        onTriggered: { saveDialog.visible = true }
    }

    // Import Obj
    Action {
        id: importObjAction
        text: "Obj"
        tooltip: "Import models in the Obj format"
        onTriggered: { importDialog.visible = true }
    }

    // Export Ply
    Action {
        id: exportPlyAction
        text: "Ply"
        tooltip: "Export models to the ply format"
        onTriggered: { exportDialog.visible = true }
    }

    // Export Ply
    Action {
        id: exportCameraDataAction
        text: "Camera Data"
        tooltip: "Export selected camera's data"
        onTriggered: { exportCameraDataDialog.visible = true }
    }

    // Properties
    Action {
        id: propAction
        text: "Properties"
        tooltip: "Modify application settings"
        onTriggered: { propDialog.visible = (propDialog.visible) ? false : true; }
    }

    // Close 
    Action {
        id: closeAction
        text: "Close"
        tooltip: "Close Feather"
        onTriggered: { Qt.quit() }
    }


    // MENU


    // New 
    MenuItem {
        action: newAction
    }

    // Open 
    MenuItem {
        action: openAction
    }

    // Save 
    MenuItem {
        action: saveAction
    }


    MenuSeparator {}

    // Import Menu
    Menu {
        id: importMenu
        title: "Import"

        
        // Obj
        MenuItem {
            action: importObjAction
        }
    }

    // Export Menu
    Menu {
        id: exportMenu
        title: "Export"

        // Obj
        MenuItem {
            action: exportPlyAction
        }
 
        // Camera Data 
        MenuItem {
            action: exportCameraDataAction
        }
    }


    MenuSeparator {}

    MenuItem {
        action: propAction
    }

    MenuSeparator {}

    // Close
    MenuItem {
        action: closeAction
    }

}
