/***********************************************************************
 *
 * Filename: RenderWindow.qml 
 *
 * Description: Displays image from renderer.
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
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4
import "common"
import feather.editors 1.0
import feather.render 1.0
import feather.attribute 1.0

Rectangle {
    id: renderWindow
    color: "grey" 
    
    property Properties properties: Null
    property bool rendering: false

    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 36
        style: ToolBarStyle {
            background: Rectangle {
                color: properties.getColor("toolBarBg")
                border.color: "black"
                border.width: 1
            }
        }

        RowLayout {
            spacing: 6

            PushButton {
                id: renderButton
                //width: 32
                //height: 32
                text: "Render"
                properties: renderWindow.properties 
            }

            Text {
                id: testLabel
                text: "test"
                width: 100
                height: 32
            }
        }

    }

    RenderImage {
        id: renderImage
        anchors.centerIn: parent
        width: 400
        height: 200
    }

    Render {
        id: render
        image: renderImage
    }

    Timer {
        id: bufferUpdateTimer
        repeat: true
        running: false
        interval: 500
        onTriggered: { render.render_buffer(1) }
    }

    Attribute {
        id: testBoolAttribute
        plugin_id: 6
        attribute_id: 1
    }

    Attribute {
        id: testUIntAttribute
        plugin_id: 6
        attribute_id: 2
    }

    Attribute {
        id: testIntAttribute
        plugin_id: 6
        attribute_id: 3
    }

    Attribute {
        id: testRealAttribute
        plugin_id: 6
        attribute_id: 4
    }

    Attribute {
        id: testStringAttribute
        plugin_id: 6
        attribute_id: 5
    }




    function render_button_pressed() {
        //var t
        //testAttribute.attribute_type(t)
        //console.log("value="+t)
        //testLabel.text = t
        //testLabel.text = testAttribute.ival

        // READ VALUES

        console.log("BOOL TEST")
        console.log("value for bool:" + testBoolAttribute.bval)
        console.log("value for bool value of uint:" + testUIntAttribute.bval)
        console.log("value for bool value of int:" + testIntAttribute.bval)
        console.log("value for bool value of real:" + testRealAttribute.bval)
        console.log("value for bool value of string:" + testStringAttribute.bval)

        console.log("UINT TEST")
        console.log("value for uint:" + testUIntAttribute.uival)
        console.log("value for uint value of bool:" + testBoolAttribute.uival)
        console.log("value for uint value of int:" + testIntAttribute.uival)
        console.log("value for uint value of real:" + testRealAttribute.uival)
        console.log("value for uint value of string:" + testStringAttribute.uival)

        console.log("INT TEST")
        console.log("value for int:" + testIntAttribute.ival)
        console.log("value for int value of bool:" + testBoolAttribute.ival)
        console.log("value for int value of uint:" + testUIntAttribute.ival)
        console.log("value for int value of real:" + testRealAttribute.ival)
        console.log("value for int value of string:" + testStringAttribute.ival)

        console.log("REAL TEST")
        console.log("value for real:" + testRealAttribute.rval)
        console.log("value for real value of bool:" + testBoolAttribute.rval)
        console.log("value for real value of uint:" + testUIntAttribute.rval)
        console.log("value for real value of int:" + testIntAttribute.rval)
        console.log("value for real value of string:" + testStringAttribute.rval)

        console.log("STRING TEST")
        console.log("value for string:" + testStringAttribute.sval)
        console.log("value for string value of bool:" + testBoolAttribute.sval)
        console.log("value for string value of uint:" + testUIntAttribute.sval)
        console.log("value for string value of int:" + testIntAttribute.sval)
        console.log("value for string value of real:" + testRealAttribute.sval)

        // SET VALUES

        testBoolAttribute.bval = false;
        testUIntAttribute.uival = 3426;
        testIntAttribute.ival = 4367;
        testRealAttribute.rval = 34.23;
        testStringAttribute.sval = "value set";

        // READ VALUES

        console.log("BOOL TEST")
        console.log("value for bool:" + testBoolAttribute.bval)
        console.log("value for bool value of uint:" + testUIntAttribute.bval)
        console.log("value for bool value of int:" + testIntAttribute.bval)
        console.log("value for bool value of real:" + testRealAttribute.bval)
        console.log("value for bool value of string:" + testStringAttribute.bval)

        console.log("UINT TEST")
        console.log("value for uint:" + testUIntAttribute.uival)
        console.log("value for uint value of bool:" + testBoolAttribute.uival)
        console.log("value for uint value of int:" + testIntAttribute.uival)
        console.log("value for uint value of real:" + testRealAttribute.uival)
        console.log("value for uint value of string:" + testStringAttribute.uival)

        console.log("INT TEST")
        console.log("value for int:" + testIntAttribute.ival)
        console.log("value for int value of bool:" + testBoolAttribute.ival)
        console.log("value for int value of uint:" + testUIntAttribute.ival)
        console.log("value for int value of real:" + testRealAttribute.ival)
        console.log("value for int value of string:" + testStringAttribute.ival)

        console.log("REAL TEST")
        console.log("value for real:" + testRealAttribute.rval)
        console.log("value for real value of bool:" + testBoolAttribute.rval)
        console.log("value for real value of uint:" + testUIntAttribute.rval)
        console.log("value for real value of int:" + testIntAttribute.rval)
        console.log("value for real value of string:" + testStringAttribute.rval)

        console.log("STRING TEST")
        console.log("value for string:" + testStringAttribute.sval)
        console.log("value for string value of bool:" + testBoolAttribute.sval)
        console.log("value for string value of uint:" + testUIntAttribute.sval)
        console.log("value for string value of int:" + testIntAttribute.sval)
        console.log("value for string value of real:" + testRealAttribute.sval)



        if(rendering) {
            render.render_stop(1)
            rendering=false
            bufferUpdateTimer.stop()
        } else {
            render.render_start(1)
            rendering=true
            bufferUpdateTimer.start()
        }
    }

    Component.onCompleted: {
        renderButton.clicked.connect(render_button_pressed)
    }
}

