/***********************************************************************
 *
 * Filename: "Properties".qml 
 *
 * Description: Holds all the properties for the UI.
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
import feather.field 1.0

Item {
    id: properties
   
    signal colorsChanged()
 
    ListModel {
        id: colorProperties
        // UI
        ListElement { name: "windowBg"; color: "#848482" }
        ListElement { name: "textBg"; color: "#F0F8FF" }
        ListElement { name: "text"; color: "#000000" }
        ListElement { name: "panel"; color: "#303030" }
        ListElement { name: "labelBg"; color: "#F0F8FF" }
        ListElement { name: "label"; color: "#000000" }

        // MENU
        ListElement { name: "menuBarBg"; color: "#848482" }
        ListElement { name: "menuPanelBg"; color: "#848482" }
        ListElement { name: "menuItemEnabledBg"; color: "#F0F8FF" }
        ListElement { name: "menuItemEnabledText"; color: "#000000" }
        ListElement { name: "menuItemEnabledHoverText"; color: "#000000" }

        // TOOLBAR 
        ListElement { name: "toolBarBg"; color: "#848482" }
 
        // BUTTON
        ListElement { name: "buttonEnabledBg"; color: "#F0F8FF" }
        ListElement { name: "buttonEnabledPressedBg"; color: "#FFFFFF" }
        ListElement { name: "buttonDisabledBg"; color: "#303030" }

        // OPTIONS 
        ListElement { name: "optionSelectedBg"; color: "#303030" }
        ListElement { name: "optionSelectedText"; color: "#FFFFFF" }
        ListElement { name: "optionPanelBg"; color: "#848482" }
        ListElement { name: "optionPanelText"; color: "#000000" }
 
        // TYPES
        ListElement { name: "boolType"; color: "#008080" }
        ListElement { name: "intType"; color: "#E2725B" }
        ListElement { name: "floatType"; color: "#318CE7" }
        ListElement { name: "doubleType"; color: "#FCF75E" }
        ListElement { name: "vertexType"; color: "#FF7F00" }
        ListElement { name: "meshType"; color: "#FF007F" }
        ListElement { name: "rgbType"; color: "#EDC9AF" }
        ListElement { name: "nodeType"; color: "#FD5800" }
        ListElement { name: "matrixType"; color: "#0F4D92" }
        // NODES
        ListElement { name: "cameraNode"; color: "#0F4D92" }
        ListElement { name: "lightNode"; color: "#EDC9AF" }
    }

    Component.onCompleted: {}

    function loadProperties() {
        colorsChanged()
    }

    function saveProperties() {

    }

    function getColor(name) {
        for(var i=0; i < colorProperties.count; i++) {
            if(colorProperties.get(i).name==name)
                return colorProperties.get(i).color
        }
        return "#FF0000"
    }

    function setColor(name,color) {
        for(var i=0; i < colorProperties.count; i++) {
            if(colorProperties.get(i).name==name)
                colorProperties.get(i).color=color
        }
        colorsChanged()
    }

    function getFieldTypeColor(type) {
        switch(type) {
            case Field.Bool: return properties.getColor("boolType"); break;
            case Field.BoolArray: return properties.getColor("boolType"); break;
            case Field.Int: return properties.getColor("intType"); break;
            case Field.IntArray: return properties.getColor("intType"); break;
            //case Field.Float: return properties.getColor("floatType"); break;
            //case Field.FloatArray: return properties.getColor("floatType"); break;
            //case Field.Double: return properties.getColor("doubleType"); break;
            //case Field.DoubleArray: return properties.getColor("doubleType"); break;
            case Field.Real: return properties.getColor("doubleType"); break;
            case Field.RealArray: return properties.getColor("doubleType"); break;
            case Field.Vertex: return properties.getColor("vertexType"); break;
            case Field.VertexArray: return properties.getColor("vertexType"); break;
            case Field.Vector: return properties.getColor("vertexType"); break;
            case Field.VectorArray: return properties.getColor("vertexType"); break;
            case Field.Mesh: return properties.getColor("meshType"); break;
            case Field.RGB: return properties.getColor("rgbType"); break;
            case Field.RGBA: return properties.getColor("rgbType"); break;
            case Field.Node: return properties.getColor("nodeType"); break;
            case Field.NodeArray: return properties.getColor("nodeType"); break;
            case Field.Matrix3x3: return properties.getColor("matrixType"); break;
            case Field.Matrix4x4: return properties.getColor("matrixType"); break;
            default: return "white";
        }
    }

    function getKeyColor(fid) {
        // generate a color based of the fid number
        // for now we'll do this with a simple switch
        switch(fid){
            case 203: return "#FF0000"
            case 204: return "#00FF00"
            case 205: return "#0000FF"
            case 206: return "#FF0000"
            case 207: return "#00FF00"
            case 208: return "#0000FF"
            case 209: return "#FF0000"
            case 210: return "#00FF00"
            case 211: return "#0000FF"
            default: return "#FFFF00"
        }
    }
}
