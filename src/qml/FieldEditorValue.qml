/***********************************************************************
 *
 * Filename: FieldEditorValue.qml 
 *
 * Description: Used to display values in the field editor. 
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
import feather.field 1.0
import feather.scenegraph 1.0

Rectangle {
    id: fieldvalue
    height: 14
   
    property alias uidKey: field.uid 
    property alias nidKey: field.nid // this if the node's number assigned by the plugin
    property alias fidKey: field.fid // this is the fields number assigned by the plugin
    //property alias label: label.text 
    property int fieldType: 0 
    property double value: {
        switch(field.type) {
            case Field.Bool:
                return field.boolVal
                break
            case Field.Int:
                return field.intVal
                break
            /*
            case Field.Float:
                return field.realVal
                break
            case Field.Double:
                return field.realVal
                break
            */
            case Field.Real:
                return Math.floor(field.realVal*100,-2)/100.00
                break
            default:
                return field.intVal
        }
    }
    property Properties properties: Null
    property alias label: label.text

    Field { id: field }

    FieldPopup { id: popup }

    //Translation { id: name }


    // LABEL

    Text {
        id: label
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: valueBox.left
        anchors.margins: 4
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 10
        //text: name.get_field_name(nidKey,fidKey);
    }    
   
    // VALUE
 
    Rectangle {
        id: valueBox
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 80
        border.color: "black"
        border.width: 1

        Text {
            id: valueText
            text: value 
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: false 
            font.pixelSize: 10
        }
    } 


    states: [
        // NORMAL
        State {
            name: "normal"
            PropertyChanges {
                target: fieldvalue 
                color: properties.getFieldTypeColor(field.type) //"lightgrey"
            }

            PropertyChanges {
                target: label 
                color: "black"
                font.bold: false
            }
 
            PropertyChanges {
                target: valueBox
                color: (field.connected) ? "deeppink" : "lightblue"
            }

            PropertyChanges {
                target: valueText
                color: "black"
                font.bold: false
            }
        },

        // HOVER
        State {
            name: "hover"
            PropertyChanges {
                target: fieldvalue 
                color: "lightblue"
            }

            PropertyChanges {
                target: label 
                color: "black"
                font.bold: true 
            }

            PropertyChanges {
                target: valueBox
                color: "midnightblue"
            }

            PropertyChanges {
                target: valueText
                color: "white"
                 font.bold: true 
            }
        },

        // PRESSED  
        State {
            name: "pressed"
            PropertyChanges {
                target: fieldvalue 
                color: "green"
            }

            PropertyChanges {
                target: label 
                color: "black"
                font.bold: true 
            }

            PropertyChanges {
                target: valueBox
                color: "limegreen"
            }

            PropertyChanges {
                target: valueText
                color: "black"
                font.bold: true 
            }
        }

    ]


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true 
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            if(mouse.button == Qt.RightButton)
                popup.popup()

            fieldvalue.state="pressed"
            fieldvalue.update()
        }

        //onPositionChanged: { }
        onReleased: { fieldvalue.state="normal" }
        onEntered: { fieldvalue.state="hover" }
        onExited: { fieldvalue.state="normal" }
        onWheel: {
            //console.log("fieldType:" + field.type)
            var offset = wheel.angleDelta.y/120.0
            //console.log("wheel delta:",wheel.pixelDelta," angle:",wheel.angleDelta," offset:",offset)
            switch(field.type) {
                case Field.Bool || Field.BoolArray: field.boolVal = (!field.boolVal) ? true : false ; break;
                case Field.Int || Field.IntArray: field.intVal = field.intVal + offset; break;
                //case Field.Float || Field.FloatArray: field.realVal = field.floatVal + 1 ; break;
                //case Field.Double || Field.DoubleArray: field.realVal = field.realVal + 0.1; valueText.text = field.realVal.toFixed(2); SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey); break;
                case Field.Real || Field.DoubleArray: field.realVal = field.realVal + (offset*0.1); valueText.text = field.realVal.toFixed(2); SceneGraph.triggerUpdate(); SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey); break;
                case Field.Vertex || Field.VertexArray: ; break;
                case Field.Vector || Field.VectorArray: ; break;
                case Field.Mesh: ; break;
                case Field.RGB || Field.RGBA: ; break;
                default: ;
            }
        }
    }

    // This is used to manually load the field's value from the node.
    // Usually this is done when another editor has changed and the feild editor needs to be updated
    function updateValue(uid,nid,fid) {
        switch(field.type) {
            case Field.Bool || Field.BoolArray: valueText.text = field.boolVal; break;
            case Field.Int || Field.IntArray: valueText.text = field.intVal; break;
            //case Field.Float || Field.FloatArray: valueText.text = field.realVal; break;
            //case Field.Double || Field.DoubleArray: valueText.text = field.realVal.toFixed(2); break;
            case Field.Real || Field.DoubleArray: valueText.text = field.realVal.toFixed(2); break;
            case Field.Vertex || Field.VertexArray: ; break;
            case Field.Vector || Field.VectorArray: ; break;
            case Field.Mesh: ; break;
            case Field.RGB || Field.RGBA: ; break;
            default: ;
        }
    }

    /*
    function updataValue() {
        console.log("Field ",field.fid," update")
        fieldvalue.update()
    }
    */

    Component.onCompleted: { 
        fieldvalue.state="normal"
        SceneGraph.nodeFieldChanged.connect(updateValue)
        //SceneGraph.updateGraph.connect(updateValue)
    }
}
