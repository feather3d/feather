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

    function render_button_pressed() {
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

