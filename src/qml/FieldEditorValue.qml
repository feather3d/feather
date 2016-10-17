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
    //width: 200
    property alias uidKey: field.uid 
    property alias nidKey: field.nid // this if the node's number assigned by the plugin
    property alias fidKey: field.fid // this is the fields number assigned by the plugin
    //property alias label: label.text 
    property int fieldType: 0 
    property double valueStep: 0.1

    property double value: {
        switch(field.type) {
            case Field.Bool:
                return field.boolVal
                break
            case Field.Int:
                return field.intVal
                break
            case Field.Real:
                return Math.floor(field.realVal*100,-2)/100.00
                break
            default:
                return field.intVal
        }
    }
    property Properties properties: Null
    property alias label: label.text
    //focus: true

    signal keyAdded(int uid)

    Field { id: field }

    FieldPopup { id: popup }

    //Translation { id: name }


    // cpos
    Field {
        id: time 
        uid: 1 //  we could use SceneGraph.get_node_by_name("time") but the time node is always at 1
        nid: 4
        fid: 7
    }

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

        IntValidator {
            id: boolValidator
            bottom: 0
            top: 1
        }

        IntValidator {
            id: intValidator
        }

        DoubleValidator {
            id: realValidator
        }

        TextInput {
            id: valueEdit
            anchors.fill: parent
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: false 
            font.pixelSize: 10
            validator: {
                switch(field.type){
                    case Field.Bool:
                        return boolValidator;
                    case Field.Int:
                        return intValidator;
                    case Field.Real:
                        return realValidator;
                    default:
                        return intValidator;
                }
            }
        }

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

    Item {
        id: keyArea
        anchors.fill: parent
        focus: true 
        Keys.onPressed: {
             console.log("FieldEditorValue - key pressed for field value - key:" + event.key + " modifier:" + event.modifiers)
             if(event.key == Qt.Key_Shift){
                console.log("shift key hit")
                valueStep = 1.0
            } else {
                valueStep = 0.1
            }
            //event.accepted = false
        }

        Keys.onReleased: {
            valueStep = 0.1
        }
    } 

    Keys.onReturnPressed: { console.log("return hit for FieldEditorValue") }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true 
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            if(mouse.button == Qt.RightButton){
                popup.time = time.realVal
                popup.value = field.realVal // we'll just use the real types for the time being
                popup.uid = uidKey
                popup.nid = nidKey
                popup.fid = fidKey
                console.log("Values sent to Field Value popup - time:",popup.time," value:",popup.value," uid:",popup.uid," nid:",popup.nid," fid:",popup.fid)
                popup.popup()
            }

            fieldvalue.state="pressed"
            fieldvalue.update()
        }

        onDoubleClicked: {
            valueEdit.visible=true
            valueText.visible=false
            valueText.focus=false
            valueEdit.focus=true
        }

        //onPositionChanged: { }
        onReleased: { fieldvalue.state="normal" }

        // NOTE!!! - on listviews, currentIndex has to be set to accept key events
        onEntered: { fieldvalue.state="hover"; view.currentIndex=index; keyArea.focus=true }

        onExited: { fieldvalue.state="normal"; keyArea.focus=false }

        onWheel: {
            //console.log("fieldType:" + field.type)
            var offset = wheel.angleDelta.y/120.0
            //console.log("wheel delta:",wheel.pixelDelta," angle:",wheel.angleDelta," offset:",offset)
            console.log("valueStep for FieldEditorValue="+valueStep)
            switch(field.type) {
                case Field.Bool || Field.BoolArray:
                    field.boolVal = (!field.boolVal) ? true : false;
                    SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey);
                break;
                case Field.Int || Field.IntArray:
                    field.intVal = field.intVal + ((valueStep*10) * offset);
                    SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey);
                break;
                case Field.Real || Field.RealArray:
                    field.realVal = field.realVal + (valueStep * offset);
                    valueText.text = field.realVal.toFixed(2);
                    SceneGraph.triggerUpdate();
                    SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey);
                break;
                case Field.Vertex || Field.VertexArray: ;
                    break;
                case Field.Vector || Field.VectorArray: ;
                    break;
                case Field.Mesh: ;
                    break;
                case Field.RGB || Field.RGBA: ;
                    break;
                default: ;
            }
        }
    }

    // This is used to manually load the field's value from the node.
    // Usually this is done when another editor has changed and the feild editor needs to be updated
    function updateValue(uid,nid,fid) {
        if(field != null){
            switch(field.type) {
                case Field.Bool || Field.BoolArray: valueText.text = field.boolVal; break;
                case Field.Int || Field.IntArray: valueText.text = field.intVal; break;
                //case Field.Float || Field.RealArray: valueText.text = field.realVal; break;
                //case Field.Double || Field.DoubleArray: valueText.text = field.realVal.toFixed(2); break;
                case Field.Real || Field.RealArray: valueText.text = field.realVal.toFixed(2); break;
                case Field.Vertex || Field.VertexArray: ; break;
                case Field.Vector || Field.VectorArray: ; break;
                case Field.Mesh: ; break;
                case Field.RGB || Field.RGBA: ; break;
                default: ;
            }
        }
    }

    function setEditValue(){
        valueText.text=valueEdit.text
        switch(field.type) {
            case Field.Bool: field.boolVal = valueText.text; SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey); break;
            case Field.Int: field.intVal = valueText.text; SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey); break;
            case Field.Real: field.realVal = valueText.text; SceneGraph.nodeFieldChanged(uidKey,nidKey,fidKey); break;
            case Field.Vertex: ; break;
            case Field.Vector: ; break;
            case Field.Mesh: ; break;
            case Field.RGB: ; break;
            default: break;
        }
        valueEdit.visible=false
        valueText.visible=true
        valueEdit.focus=false
    }

    Component.onCompleted: { 
        fieldvalue.state="normal"
        SceneGraph.nodeFieldChanged.connect(updateValue)
        //SceneGraph.updateGraph.connect(updateValue)
        valueEdit.accepted.connect(setEditValue)
    }
}
