/***********************************************************************
 *
 * Filename: TimeSlider.qml 
 *
 * Description: Handles zooming in on the timeline. 
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

Rectangle {
    id: slider 
    color: "darkgrey"
    border.width: 1
    radius: 2
    height: 20
    width: 400
 
    // all these times are in seconds
    property double stime: 0
    property double etime: 10
    property double spos: 2
    property double epos: 9
    property int cx: 0 // current click location

    signal barChanged()

    SMPTE {
        id: spos_timecode
        position: spos
     }
  
    SMPTE {
        id: epos_timecode
        position: epos
    }

    Rectangle {
        id: viewable
        color: "lightgrey"
        border.width: 1
        radius: 5
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: (slider.width/(etime-stime)) * (epos-spos)
        x: (width / (etime - stime)) * spos
 
        Rectangle {
            id: viewable_start
            color: "blue"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 4
            border.width: 1
            width: height
            radius: height/2
 
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true 
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onPositionChanged: {
                    if(mouse.buttons == Qt.LeftButton) {
                        var pos = mapToItem(slider,mouse.x,mouse.y,viewable.width,viewable.height)
                        var pps = slider.width / (etime - stime)
                        spos = (pos.x-width)/pps
                        if(spos < stime)
                            spos = stime
                        if(spos > epos)
                            spos = epos - 10
                    }
                }

                onPressed: { viewable_start.color = "yellow"; var pos = mapToItem(slider,mouse.x,mouse.y,viewable.width,viewable.height); cx = pos.x }
                onReleased: { viewable_start.color="blue" }
                onEntered: { } 
                onExited: { }
                onWheel: { }
            }
        }

        Text {
            id: viewable_start_label
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: viewable_start.right
            anchors.margins: 4
            width: 100
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 10
            text: spos_timecode.timecode
        }    
 
        Text {
            id: viewable_end_label
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: viewable_end.left
            anchors.margins: 4
            width: 100
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 10
            text: epos_timecode.timecode 
        }    
        
        Rectangle {
            id: viewable_end
            color: "blue"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 4
            border.width: 1
            width: height
            radius: height/2
 
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true 
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onPositionChanged: {
                    if(mouse.buttons == Qt.LeftButton) {
                        var pos = mapToItem(slider,mouse.x,mouse.y,viewable.width,viewable.height)
                        var pps = slider.width / (etime - stime)
                        epos = (pos.x+width)/pps
                        if(epos > etime)
                            epos = etime
                        if(epos < spos)
                            epos = spos + 10
                    }
                }

                onPressed: { viewable_end.color = "yellow"; var pos = mapToItem(slider,mouse.x,mouse.y,viewable.width,viewable.height); cx = pos.x }
                onReleased: { viewable_end.color="blue" }
                onEntered: { } 
                onExited: { }
                onWheel: { }
            }

        }

        MouseArea {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: viewable_end.left
            anchors.left: viewable_start.right  
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onPositionChanged: {
                //console.log("slider position changed")
                if(mouse.buttons == Qt.LeftButton) {
                    var pps = slider.width / (etime - stime)
                    var dsec = (mouse.x - cx) / pps
                    if((spos + dsec) < stime)
                        spos = stime
                    else if((epos + dsec) > etime)
                        epos = etime
                    else { 
                        spos += dsec
                        epos += dsec
                    }
                    viewable.x = pps * spos
                    barChanged()
                }
            }

            onPressed: { cx = mouse.x }
            onReleased: { }
            onEntered: { } 
            onExited: { }
            onWheel: { }
        }
    }
    
    // this is need since the width at Component.onCompleted is not correct
    onWidthChanged: {
        viewable.x = (width / (etime - stime)) * spos
    }

    onSposChanged: {
        viewable.x = (width / (etime - stime)) * spos
    }

    Component.onCompleted: {}
}
