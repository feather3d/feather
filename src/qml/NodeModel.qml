/***********************************************************************
 *
 * Filename: NodeTypeModel.qml 
 *
 * Description: Holds all the type of all the different nodes that can be stored in the scenegraph. This is not to be the specific node - this model will hold a type like Light but many different light nodes can be under this type. 
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
import feather.node 1.0

ListModel {
    //ListElement { text: "Null"; type: Node.Null }// will get an error if you use Null as an ListElement
    // Empty
    ListElement { text: "Empty"; nid: 1; type: Node.Empty }
    ListElement { text: "Camera"; nid: 2; type: Node.Camera }
    //ListElement { text: "Light"; type: Node.Light }
    //ListElement { text: "Texture"; type: Node.Texture }
    //ListElement { text: "Modifier"; type: Node.Modifier }
    //ListElement { text: "Deformer"; type: Node.Deformer }
    // Manipulator
    ListElement { text: "Time"; nid: 4; type: Node.Time }
    //ListElement { text: "Transform"; nid: 7; type: Node.Manipulator }
    // Tool
    ListElement { text: "Multiply"; nid: 5; type: Node.Tool }
    ListElement { text: "Divide"; nid: 6; type: Node.Tool }
    //ListElement { text: "Shader"; type: Node.Shader }
    // Object
    //ListElement { text: "Empty"; nid: 0; type: Node.Object }
    // Curve
    //ListElement { text: "Curve"; type: Node.Curve }
    // Polygon
    ListElement { text: "Shape"; nid: 320; type: Node.Polygon }
    ListElement { text: "Plane"; nid: 321; type: Node.Polygon }
    ListElement { text: "Cube"; nid: 322; type: Node.Polygon }
    ListElement { text: "SubDiv"; nid: 323; type: Node.Polygon }
    ListElement { text: "Mesh"; nid: 324; type: Node.Polygon }

    // ANIMATION PLUGIN
    ListElement { text: "Track"; nid: 420; type: Node.Animation }
    ListElement { text: "Bone"; nid: 421; type: Node.Animation }
    ListElement { text: "Morph"; nid: 426; type: Node.Animation }

    // DEFORMER PLUGIN
    ListElement { text: "Cluster"; nid: 440; type: Node.Deformer}

}
