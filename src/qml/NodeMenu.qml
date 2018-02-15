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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2 
import feather.command 1.0
import feather.scenegraph 1.0
import feather.tools 1.0
import feather.node 1.0
import "common"

Menu {
    id: nodeMenu 
    title: "Node"
    visible: true 

    property Properties properties: Null

    style: FMenuStyle { properties: nodeMenu.properties } 
    NodeTypeModel { id: typeModel }
    NodeModel { id: nidModel }

    Tools { id: tools }

    // ACTIONS

    // New 
    Action {
        id: removeNodeAction
        text: "Remove"
        tooltip: "Clear the scene"
        onTriggered: { SceneGraph.clear() }
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



    // COMMANDS
    /*
    Command {
        id: addNode 
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
        id: removeNode 
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
    */

    // MENU

    // Add Node
 
    Menu {
        id: addNodeMenu
        title: "Add"
        // NODE TYPES 
        Menu {
            title: "Null" 
            id: nullNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Null){ nullNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Null){ nullNodeMenu.removeItem(object) }
            }
        }


        MenuItem {
            text: "Empty" 
        }

        Menu {
            title: "Camera" 
            id: cameraNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Camera){ cameraNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Camera){ cameraNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Time" 
            id: timeNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Time){ timeNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Time){ timeNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Light" 
            id: lightNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Light){ lightNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Light){ lightNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Texture" 
            id: textureNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Texture){ textureNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Texture){ textureNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Animation" 
            id: animationNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Animation){ animationNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Animation){ animationNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Modifier" 
            id: modifierNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Modifier){ modifierNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Modifier){ modifierNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Deformer" 
            id: deformerNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Deformer){ deformerNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Deformer){ deformerNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Manipulator" 
            id: manipulatorNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Manipulator){ manipulatorNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Manipulator){ manipulatorNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Shader" 
            id: shaderNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Shader){ shaderNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Shader){ shaderNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Object" 
            id: objectNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Object){ objectNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Object){ objectNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Curve" 
            id: curveNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Curve){ curveNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Curve){ curveNodeMenu.removeItem(object) }
            }
        }

        Menu {
            title: "Polygon" 
            id: polygonNodeMenu
            style: mainMenuStyle

            Instantiator {
                model: nidModel

                NodeMenuItem {
                    nid: model.nid
                    ntype: model.type
                    name: model.text
                }

                onObjectAdded: if(object.ntype==Node.Polygon){ polygonNodeMenu.insertItem(index,object) }
                onObjectRemoved: if(object.type==Node.Polygon){ polygonNodeMenu.removeItem(object) }
            }
        }

        /*
        MenuItem {
            text: "Shape" 
        }

        MenuItem {
            text: "Tool" 
        }
        */
    }

    MenuSeparator {}

    MenuItem {
        action: removeNodeAction
    }
}
