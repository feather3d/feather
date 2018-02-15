/***********************************************************************
 *
 * Filename: FieldValue.qml 
 *
 * Description: Reads and modifies fields. 
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
//import feather.scenegraph 1.0
import feather.field 1.0

Rectangle {
    id: box 
    height: 50
    border.width: 1
    radius: 2
    color: "darkgrey"

    property alias uid: field.uid 
    property alias nid: field.nid
    property alias fid: field.fid 
    property alias boolValue: field.boolVal
    property alias intValue: field.intVal
    property alias realValue: field.realVal
    property double valueStep: 0.1

    Field {
        id: field
    }

    TextEdit {
        id: label 
        anchors.fill: parent
        anchors.margins: 2
        text: {
            switch(field.type) {
                case Field.Bool:
                    return field.boolVal
                    break
                case Field.Int:
                    return field.intVal
                    break
                case Field.Real:
                    return field.realVal
                    break
                default:
                    return field.intVal
            }
        }
        font.pixelSize: 14
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
        inputMethodHints: (field.type == Field.Int) ? Qt.ImhDigitsOnly : Qt.ImhFormattedNumbersOnly
    }

    states: [
        // NORMAL
        State {
            name: "Normal"

            PropertyChanges {
                target: label 
                color: "black"
                font.bold: false
            }
 
            PropertyChanges {
                target: box
                color: (field.connected) ? "deeppink" : "lightblue"
            }
        },

        // HOVER
        State {
            name: "Hover"

            PropertyChanges {
                target: label 
                color: "black"
                font.bold: true 
            }

            PropertyChanges {
                target: box 
                color: "midnightblue"
            }
        },

        // PRESSED  
        State {
            name: "Pressed"

            PropertyChanges {
                target: label 
                color: "black"
                font.bold: true 
            }

            PropertyChanges {
                target: box 
                color: "limegreen"
            }
        }
    ]

    Item {
        id: keyArea
        anchors.fill: parent
        focus: false 

        Keys.onEnterPressed: {
            switch(field.type) {
                case Field.Bool:
                    field.boolVal = box.value
                    break
                case Field.Int:
                    field.intVal = box.value
                    break
                case Field.Real:
                    field.realVal = box.value
                    break
                default:
                    field.intVal = box.value
            }
        }

        Keys.onPressed: {
             console.log("key pressed for field value - key:" + event.key + " modifier:" + event.modifiers)
             if(event.key == Qt.Key_Shift){
                console.log("shift key hit")
                valueStep = 1.0
            } else {
                valueStep = 0.1
            }
            event.accepted = false
        }

    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true 
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            //if(mouse.button == Qt.RightButton)
            //    popup.popup()

            switch(field.type) {
                case Field.Bool:
                    return field.boolVal
                    break
                case Field.Int:
                    return field.intVal
                    break
                case Field.Real:
                    return field.realVal
                    break
                default:
                    return field.intVal
            }
 
            box.state="pressed"
            box.update()

        }

        //onPositionChanged: { }
        onReleased: { box.state="Normal" }
        onEntered: { box.state="Hover"; keyArea.focus=true }
        onExited: { box.state="Normal"; keyArea.focus=false }
        onWheel: {
            console.log("valueStep="+valueStep)
            switch(field.type) {
                case Field.Bool:
                    field.boolVal = (!field.boolVal) ? 1 : 0
                    break
                case Field.Int:
                    field.intVal = field.intVal + (valueStep * 100)
                    break
                case Field.Real:
                    field.realVal = field.realVal + valueStep
                    break
                default:
                    field.intVal = field.intVal + 1
            }
            updateValue()
        }
    }

    function updateValue() {
        switch(field.type) {
            case Field.Bool:
                label.text = field.boolVal
                break
            case Field.Int:
                label.text = field.intVal
                break
            case Field.Real:
                label.text = field.realVal
                break
            default:
                label.text = field.intVal
        }
    }

    Component.onCompleted: { box.state = "Normal" }
}
