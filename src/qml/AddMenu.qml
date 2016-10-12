/***********************************************************************
 *
 * Filename: AddMenu.qml 
 *
 * Description: Container for all menu items that will add a new node to the scenegraph.
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
import feather.scenegraph 1.0
import feather.node 1.0
import feather.command 1.0

Menu {
    id: addMenu
    title: "Add"
    visible: true

    property Properties properties: Null

    style: MenuStyle { properties: addMenu.properties }

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

    // Add Polygon Plane 
    Action {
        id: addPolygonPlaneAction
        text: "Plane"
        tooltip: "Add polygon plane to the scenegraph"
        onTriggered: {
            var shape
            var plane
            plane = SceneGraph.add_node(321,"Plane01")
            shape = SceneGraph.add_node(320,"PlaneShape01")
            var suid = SceneGraph.selected_node()
            SceneGraph.connect_nodes(suid,202,plane,201)
            SceneGraph.connect_nodes(plane,202,shape,201)
            SceneGraph.connect_nodes(plane,3,shape,1)
            SceneGraph.triggerUpdate()
            SceneGraph.nodeUpdateDrawItems(plane)
            SceneGraph.nodeUpdateDrawItems(shape)
            console.log("plane node = " + plane)
            console.log("shape node = " + shape)
        }
    }

    // Add Polygon Cube
    Action {
        id: addPolygonCubeAction
        text: "Cube"
        tooltip: "Add polygon cube to the scenegraph"
        onTriggered: {
            var cube = SceneGraph.add_node(322,"cube")
            var shape = SceneGraph.add_node(320,"cubeShape")
            // connect parent/child connections
            SceneGraph.connect_nodes(0,202,cube,201)
            SceneGraph.connect_nodes(cube,202,shape,201)
            // mesh connections
            SceneGraph.connect_nodes(cube,4,shape,1)
            SceneGraph.triggerUpdate()
            SceneGraph.nodeUpdateDrawItems(cube)
            SceneGraph.nodeUpdateDrawItems(shape)
        }
    }


    // MENU

    // Polygon Menu
    Menu {
        id: polygonMenu
        title: "Polygon"

        // Plane 
        MenuItem {
            action: addPolygonPlaneAction
        }

        // Cube 
        MenuItem {
            action: addPolygonCubeAction
        }

    }

    // Light Menu
    Menu {
        id: lightMenu
        title: "Light"

        // 
        //MenuItem {
        //    action: 
        //}

    }

    // Camera Menu
    Menu {
        id: cameraMenu 
        title: "Camera"

        // 
        //MenuItem {
        //    action: 
        //}

    }

    MenuSeparator {}

    // Base Menu
    Menu {
        id: baseMenu 
        title: "Base"

        // 
        //MenuItem {
        //    action: 
        //}

    }

}
