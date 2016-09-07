/***********************************************************************
 *
 * Filename: KeyTrack.qml 
 *
 * Description: Draws the keyframe track. 
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
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import feather.scenegraph 1.0
import feather.field 1.0

Rectangle {
    id: frame 
    color: "grey"
    property int uid: 0 // track node uid
    property double stime: 0.0 // seconds
    property double etime: 10.0 // seconds
    property double fps: 24.0
    property double cpos: 5 
    property var list: [] 
    property double minVal: 0 // current minimum displayed value
    property double maxVal: 10 // current maximum displayed value
    property int cid: 0 // used as curve model id when inserting key positions

    signal startTimeChanged(double time)
    signal endTimeChanged(double time)
    signal currentTimeChanged(double time)

    Field { id: keyframe; nid: 420 }

    ListModel { id: curvemodel }

    onStimeChanged: {
        track.requestPaint()
    }

    onEtimeChanged: {
        track.requestPaint()
    }

    onCposChanged: {
        track.requestPaint()
    }

    Canvas {
        id: track 
        contextType: "2d"
        anchors.fill: parent
        anchors.margins: 2
        Layout.fillWidth: true

        onPaint: {
            context.reset()

            // draw track time lines
            context.beginPath()
            context.strokeStyle = "#000000"
            draw_track(context)

            curvemodel.clear()
            cid = 0

            // draw keys
            context.beginPath()
            context.strokeStyle = "#fb7e14"
            if(uid) {
                for(var i=0; i < list.length; i++)
                    draw_key(context,list[i])
            }

            // draw curve
            context.beginPath()
            context.strokeStyle = "#054552"
            draw_curve(context)

        }


        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            property bool moveframe: false
            property int mouseX: 0
            property int mouseY: 0

            onPressed: {
                moveframe = true
                mouseX = mouse.x
                mouseY = mouse.y
            }

            onPositionChanged: {
                if(moveframe){
                    var dX = mouseX - mouse.x
                    var dY = mouseY - mouse.y
                    minVal += dY/10.0
                    maxVal += dY/10.0
                    // Note - send back a signal to the timebar to update it's start and end times 
                    var length = (etime - stime)
                    var pps = width/length // pixels per second 
                    stime += dX/pps
                    etime += dX/pps
                    startTimeChanged(stime)
                    endTimeChanged(etime)
                    //console.log("y:",mouse.y,", min:",minVal,", max:",maxVal)
                }
                mouseX = mouse.x
                mouseY = mouse.y
                track.requestPaint()
            }

            onReleased: {
                moveframe = false
            }
        }
    }

    function draw_key(context,keyuid) {
        keyframe.uid = keyuid
        keyframe.fid = 4
        var val = keyframe.intVal
        keyframe.fid = 3
        var keyTime = keyframe.realVal
        var ppv = height/(maxVal - minVal)
        //context.moveTo(100,ppv*(maxVal-val))
        var length = (etime - stime)
        var pps = width/length // pixels per second 
        var keyX = (keyTime - stime) * pps
        //var keyX = (stime-keyTime) * pps
        var keyY = ppv*(maxVal-val)
        //console.log("key uid:",keyuid,", type=",keyframe.type,", x=",keyX,", y=",keyY,", value=",keyTime)
        context.rect(keyX-4,keyY-4,8,8)
        context.stroke()
        curvemodel.insert(cid,{"x":keyX,"y":keyY})
        cid++
        //console.log("keyframe value:",val)
    }

    function draw_curve(context) {
        if(curvemodel.count){
            context.moveTo(curvemodel.get(0).x,curvemodel.get(0).y)
            for(var i=1; i < curvemodel.count; i++){
                context.lineTo(curvemodel.get(i).x,curvemodel.get(i).y)
            }
            context.stroke()
        }
    }

    function draw_track(context) {
        var length = (etime - stime)
        var framecount = length * fps
        var pps = width/length // pixels per second 
        var ppf = pps/fps // pixels per frame
        var fm = 100/ppf // frame multiplier
        var spf = 1.0/fps // seconds per frame
        var frameX = (stime - Math.floor(stime/spf)) * pps
        var secondX = (Math.floor(stime) - stime) * pps 
        var cposX = (cpos - stime) * pps
        var cframe = Math.floor(cpos*fps)

        var csec = Math.floor(stime)
        while(secondX < width) {
            // draw lines
            context.strokeStyle = "#000000"
            context.lineWidth = 1
            context.moveTo(secondX,0)
            context.lineTo(secondX,height)
            context.stroke()
            // display the frame number
            context.fillText(csec,secondX,height-10)
            secondX = secondX + pps
            csec += 1
        }

        // cpos 
        context.beginPath()
        context.strokeStyle = "#ff0000"
        context.lineWidth = ppf 
        context.moveTo(cposX,0)
        context.lineTo(cposX,height)
        context.stroke()

    }

    // update the track keys
    function load_keys() {
        // get all the key uids
        var tlist = SceneGraph.connected_uids(uid,4) 
        for(var i=0; tlist.length > i; i++){
            console.log("adding ",tlist[i]," to list")
            list[i] = tlist[i]
        } 
        console.log("there are ",list.length," keys")
    }

    function draw_keys() {

    }

    function update_track() {
        load_keys()
        track.requestPaint()
    }

    function paint_track() {
        track.requestPaint()
    }
}
