/***********************************************************************
 *
 * Filename: ImageButton.qml 
 *
 * Description: Generic button with image. 
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
import QtQuick.Controls 1.4

Rectangle {
    id: button 
    width: 50 
    height: 30 
    border.width: 1
    //property alias text: label.text
    property bool checked: false
    property ExclusiveGroup exclusiveGroup: null
    property alias imageNormal: normal_image.source
    property alias imageActive: active_image.source
    property alias imageHover: hover_image.source
    property alias imageDisabled: disabled_image.source

    color: (checked) ? "#444444" : "lightgrey"

    signal buttonPressed()

    signal buttonReleased()

    signal buttonClicked()

    Image {
        id: normal_image 
        anchors.fill: parent
        visible: false
        sourceSize.width: 32 
        sourceSize.height: 32 
    }

    Image {
        id: active_image 
        anchors.fill: parent
        visible: false
        sourceSize.width: 32 
        sourceSize.height: 32 
    }

    Image {
        id: hover_image 
        anchors.fill: parent
        visible: false
        sourceSize.width: 32 
        sourceSize.height: 32 
    }

    Image {
        id: disabled_image 
        anchors.fill: parent
        visible: false
        sourceSize.width: 32 
        sourceSize.height: 32 
    }

    states: [
        // NORMAL
        State {
            name: "Normal"
            PropertyChanges {
                target: normal_image 
                visible: true 
            }

            PropertyChanges {
                target: active_image 
                visible: false 
            }

            PropertyChanges {
                target: hover_image 
                visible: false 
            }

            PropertyChanges {
                target: disabled_image 
                visible: false 
            }
        },

        // ACTIVE 
        State {
            name: "Active"
            PropertyChanges {
                target: normal_image 
                visible: false 
            }

            PropertyChanges {
                target: active_image 
                visible: true 
            }

            PropertyChanges {
                target: hover_image 
                visible: false 
            }

            PropertyChanges {
                target: disabled_image 
                visible: false 
            }
        },

        // HOVER 
        State {
            name: "Hover"
            PropertyChanges {
                target: normal_image 
                visible: false 
            }

            PropertyChanges {
                target: active_image 
                visible: false 
            }

            PropertyChanges {
                target: hover_image 
                visible: true 
            }

            PropertyChanges {
                target: disabled_image 
                visible: false 
            }
        },

        // DISABLED 
        State {
            name: "Disabled"
            PropertyChanges {
                target: normal_image 
                visible: false 
            }

            PropertyChanges {
                target: active_image 
                visible: false 
            }

            PropertyChanges {
                target: hover_image 
                visible: false 
            }

            PropertyChanges {
                target: disabled_image 
                visible: true 
            }
        }

    ]

    /*
    Text {
        id: label
        color: (checked) ? "white" : "black"
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
        text: ">>" 
    }    
    */

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onPressed: {
            buttonPressed()
        }

        onReleased: {

            buttonReleased()
        }

        onClicked: {
            button.state="Active"
            buttonClicked()                
        }

        onEntered: {
            button.state="Hover"
        }

        onExited: {
            button.state=(checked) ? "Active" : "Normal"
        }
    }

    onExclusiveGroupChanged: {
        if(exclusiveGroup)
            exclusiveGroup.bindCheckable(button)
    }

    Component.onCompleted: { button.state="Normal" }
}
