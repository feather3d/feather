/***********************************************************************
 *
 * Filename: TimeCode.qml 
 *
 * Description: Displays the time position. 
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

Rectangle {
    id: window
    width: 200 
    height: 20 
    color: "black"

    property alias timeSize: label.font.pixelSize
    property alias timeColor: label.color
    property int mode: 0 // 0=stopped, 1=playing
    property double pos: 0 // in seconds 
    property double fps: 24

    SMPTE {
        id: smpte
        position: window.pos
        fps: window.fps
    }

    Text {
        id: label
        color: (mode) ? "lime" : "yellow"
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
        text: smpte.timecode 
    }    

}
