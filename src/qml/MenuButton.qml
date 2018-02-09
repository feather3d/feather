/***********************************************************************
 *
 * Filename: MenuButton.qml 
 *
 * Description: Used in menus to select items. 
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

import QtQuick 2.1
import feather.field 1.0

Rectangle {
    id: button

    property alias label: label.text
    property int ftype: 0 // this is the feather::field::Type enum value
    property int sgId: 0
    property int fieldId: 0
    property int nodeId: 0

    height: 15
    width: parent.width
    color: "lightgrey"
    border.color: "black"
    border.width: 1

    signal connectionButtonPressed(int button, int uid, int nid, int fid)
    signal connectionButtonReleased(int button, int uid, int nid, int fid)
    signal connectionButtonClicked(int button, int uid, int nid, int fid)

    Text {
        id: label
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        //font.bold: true 
        font.pixelSize: 10
        color: "black"
    }

    states: [

        // NORMAL
        State {
            name: "normal"
            PropertyChanges {
                target: button 
                color: get_type_color(type) 
            }
        },

        // HOVER
        State {
            name: "hover"
            PropertyChanges {
                target: button 
                color: "lightblue"
            }
        },

        // PRESSED  
        State {
            name: "pressed"
            PropertyChanges {
                target: button 
                color: "green"
            }
        }

    ]


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true 
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true

        onPressed: {
            mouse.accepted=false; connectionButtonPressed(mouse.button,sgId,nodeId,fieldId)
        }

        onReleased: { connectionButtonReleased(mouse.button,sgId,nodeId,fieldId); button.state="normal" }
        onClicked: { connectionButtonClicked(mouse.button,sgId,nodeId,fieldId); button.state="normal" }
        onEntered: { button.state="hover" }
        onExited: { button.state="normal" }
    }

    Component.onCompleted: {
        button.state="normal" 
    }

    function get_type_color(type) {
        switch(type)
        {
            case 0: return "red"; break; // Null
            case Field.Bool: return "blue"; break;
            case Field.Int: return "lightgreen"; break;
            case Field.Float: return "yellow"; break;
            case Field.Vertex: return "cyan"; break;
            case Field.Vector: return "pink"; break;
            case Field.Mesh: return "magenta"; break;
            default: return "white";
        }
    }
}
