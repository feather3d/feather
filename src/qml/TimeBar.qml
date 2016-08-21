/***********************************************************************
 *
 * Filename: TimeBar.qml 
 *
 * Description: Displays the time and keyframes. 
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
    id: timebar 
    color: "darkgrey"

    width: 500
    height: 20

    property double stime: 0 // seconds
    property double etime: 10 // seconds 
    property double cpos: 5 // seconds
    property int track_uid: 0
    property int display: 0 // 0=frames, 1=seconds, 2=smpte
    property double fps: 24

    Canvas {
        id: bar 
        contextType: "2d"
        anchors.fill: parent
        anchors.margins: 2

        onPaint: {
            context.reset()

            var length = (etime - stime)
            var framecount = length * fps
            var pps = width/length // pixels per second 
            var ppf = pps/fps // pixels per frame
            var fm = 100/ppf // frame multiplier
            var spf = 1.0/fps // seconds per frame
            var frameX = (stime - Math.floor(stime/spf)) * pps
            var secondX = (stime - Math.floor(stime)) * pps 
            var cposX = (cpos - stime) * pps
            var cframe = Math.floor(cpos*fps)

            // frames 

            context.beginPath()
            while(frameX < width) {
                // draw lines
                context.strokeStyle = "#444444"
                context.lineWidth = 1
                context.moveTo(frameX,(height/2))
                context.lineTo(frameX,height)
                context.stroke()
                frameX = frameX + ppf
            }
            context.stroke()
 
            // seconds 

            context.beginPath()
            while(secondX < width) {
                // draw lines
                context.strokeStyle = "#000000"
                context.lineWidth = 1
                context.moveTo(secondX,0)
                context.lineTo(secondX,height)
                context.stroke()
                secondX = secondX + pps
            }
            context.stroke()

            // keys
            if(track_uid != 0){
                // get all the key uids
                keys = SceneGraph.connections(track_uid,4)
                // TODO 
            }

            // cpos 

            context.beginPath()
            context.strokeStyle = "#ff0000"
            context.lineWidth = ppf 
            context.moveTo(cposX,0)
            context.lineTo(cposX,height)
            context.stroke()

            // display the frame number
            context.fillText(cframe,cposX+4,height/2)

        }
 
    }

    onStimeChanged: { updateBar() }

    onEtimeChanged: { updateBar() }

    function updateBar() {
        bar.requestPaint()
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true 
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            //if(mouse.button == Qt.RightButton)
            // set cpos
            var length = (etime - stime)
            var pps = width/length // pixels per second 
            cpos = stime + ( (width - (width - mouse.x)) / pps)
            bar.requestPaint()            
        }

        onPositionChanged: {
            //console.log("changed")
            if(mouse.buttons == Qt.LeftButton) {
                var length = (etime - stime)
                var pps = width/length // pixels per second 
                cpos = stime + ( (width - (width - mouse.x)) / pps)
                bar.requestPaint()
            }
        }

        onReleased: { }
        onEntered: { } 
        onExited: { }
        onWheel: { }
    }
}
