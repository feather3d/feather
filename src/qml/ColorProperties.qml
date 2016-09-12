/***********************************************************************
 *
 * Filename: ColorProperties.qml 
 *
 * Description: Container for all the application color properties.
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
import QtQuick.Layouts 1.1


Rectangle {
    id: colorProperties
    visible: true
    anchors.fill: parent
    color: "grey"
    property Properties properties: Null

    Column {
        spacing: 10

        // UI 

        Rectangle {
            width: colorProperties.width
            height: 16
            color: "lightgrey" 
            border.color: "black"
            border.width: 1
            
            Text {
                text: "UI"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: false 
                font.pixelSize: 12
                color: "black"
            }
        }

        Column {
            spacing: 4

            ColorProperty { properties: colorProperties.properties; name: "windowBg"; label: "Window BG" }
            ColorProperty { properties: colorProperties.properties; name: "panel"; label: "Panel" }
            ColorProperty { properties: colorProperties.properties; name: "labelBg"; label: "Label BG" }
            ColorProperty { properties: colorProperties.properties; name: "label"; label: "Label" }
            ColorProperty { properties: colorProperties.properties; name: "textBg"; label: "Text BG" }
            ColorProperty { properties: colorProperties.properties; name: "text"; label: "Text" }

            // menu
            ColorProperty { properties: colorProperties.properties; name: "menuBarBg"; label: "Menu Bar BG" }
            ColorProperty { properties: colorProperties.properties; name: "menuPanelBg"; label: "Menu Panel BG" }
            ColorProperty { properties: colorProperties.properties; name: "menuItemEnabledBg"; label: "Menu Item Enabled BG" }
            ColorProperty { properties: colorProperties.properties; name: "menuItemEnabledText"; label: "Menu Item Enabled Text" }
            ColorProperty { properties: colorProperties.properties; name: "menuItemEnabledHoverText"; label: "Menu Item Enabled Hover Text" }
            // toolbar 
            ColorProperty { properties: colorProperties.properties; name: "toolBarBg"; label: "Toolbar BG" }

            // buttons
            ColorProperty { properties: colorProperties.properties; name: "buttonEnabledBg"; label: "Button Enabled BG" }
            ColorProperty { properties: colorProperties.properties; name: "buttonEnabledPressedBg"; label: "Button Enabled Pressed BG" }
            ColorProperty { properties: colorProperties.properties; name: "buttonDisabledBg"; label: "Button Disabled BG" }

            // options
            ColorProperty { properties: colorProperties.properties; name: "optionSelectedBg"; label: "Option Selected BG" }
            ColorProperty { properties: colorProperties.properties; name: "optionSelectedText"; label: "Option Selected Text" }
            ColorProperty { properties: colorProperties.properties; name: "optionPanelBg"; label: "Option Panel BG" }
            ColorProperty { properties: colorProperties.properties; name: "optionPanelText"; label: "Option Panel Text" }
 
        } 

        // TYPES

        Rectangle {
            width: colorProperties.width
            height: 16
            color: "lightgrey" 
            border.color: "black"
            border.width: 1
            
            Text {
                text: "TYPES"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: false 
                font.pixelSize: 12
                color: "black"
            }
        }

        Column {
            spacing: 4

            ColorProperty { properties: colorProperties.properties; name: "boolType"; label: "Boolean" }
            ColorProperty { properties: colorProperties.properties; name: "intType"; label: "Int" }
            ColorProperty { properties: colorProperties.properties; name: "floatType"; label: "Float" }
            ColorProperty { properties: colorProperties.properties; name: "doubleType"; label: "Double" }
            ColorProperty { properties: colorProperties.properties; name: "vertexType"; label: "Vertex" }
            ColorProperty { properties: colorProperties.properties; name: "meshType"; label: "Mesh" }
            ColorProperty { properties: colorProperties.properties; name: "rgbType"; label: "RGB" }
            ColorProperty { properties: colorProperties.properties; name: "nodeType"; label: "Node" }
            ColorProperty { properties: colorProperties.properties; name: "matrixType"; label: "Matrix" }
       } 

        // TYPES

        Rectangle {
            width: colorProperties.width
            height: 16
            color: "lightgrey" 
            border.color: "black"
            border.width: 1
            
            Text {
                text: "NODES"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: false 
                font.pixelSize: 12
                color: "black"
            }
        }

        Column {
            spacing: 4

            ColorProperty { properties: colorProperties.properties; name: "cameraNode"; label: "Camera" }
            ColorProperty { properties: colorProperties.properties; name: "lightNode"; label: "Light" }
       } 

    }

    Component.onCompleted: { }
}
