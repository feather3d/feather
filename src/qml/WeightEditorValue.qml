/***********************************************************************
 *
 * Filename: WeightEditorValue.qml 
 *
 * Description: Used to edit the values in the weight editors.
 *
 * Copyright (C) 2016 Richard Layman, rlayman2000@yahoo.com 
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
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import feather.scenegraph 1.0
import feather.field 1.0
import feather.scenegraph 1.0

Rectangle {
    id: widget 
    height: 22

    property alias name: label_text.text
    property double uid: 0 
    property double fid: 0 
    property int weightId: 0
    color: "lightblue"
    border.color: "black"
    border.width: 1

    Field { id: field }

    // Validator
    DoubleValidator {
        id: doublevalidator
        bottom: 0.0
        top: 1.0
    }
 
    // Label
    Rectangle {
        id: label
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: (label_text.contentWidth>100) ? label_text.contentWidth : 100
        anchors.margins: 2
        color: "grey"
        //border.color: "black"
        //border.width: 1

        Text {
            id: label_text
            anchors.fill: parent
            anchors.margins: 4
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 10
        }
    }

    // Slider
    Slider {
        id: slider
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: label.right
        anchors.right: value.left
        anchors.margins: 2
        orientation: Qt.Horizontal
        minimumValue: 0.0
        maximumValue: 1.0
        //stepSize: 0.1

        onValueChanged: {
            value.text = slider.value.toFixed(3);
            field.realArrayVal[weightId] = slider.value
            SceneGraph.triggerUpdate()
        }
    }

    // Value
    TextField {
        id: value
        width: 50
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 2
        validator: doublevalidator 
        text: "0.0"

        onTextChanged: {
            slider.value = text
        }
    }

    onNameChanged: { load_value() }

    function load_value() {
        field.uid = uid
        field.fid = fid
        var value = field.realArrayVal
        if(value.length < weightId)
            slider.value = 0.0
        else
            slider.value = value[weightId]
    }
}
