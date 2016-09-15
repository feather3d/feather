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
    //property var list: [] 
    property double minVal: 0 // current minimum displayed value
    property double maxVal: 10 // current maximum displayed value
    //property int cid: 0 // used as curve model id when inserting key positions

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

    onUidChanged: {
        update_track()
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
            draw_track(context)

            // draw keys
            if(uid) {
                for(var i=0; i < curvemodel.count; i++)
                    draw_key(context,curvemodel.get(i))
            }

            // draw curve
            draw_curve(context)
        }


        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            property bool movekey: false
            property bool moveframe: false
            property int mouseX: 0
            property int mouseY: 0
            hoverEnabled: true

            onDoubleClicked: {
                var selecttolerance = 4
                for(var i=0; i < curvemodel.count; i++){
                    var key = curvemodel.get(i)
                    if( (key.x + selecttolerance) >= mouse.x &&
                        (key.x - selecttolerance) <= mouse.x &&
                        (key.y + selecttolerance) >= mouse.y &&
                        (key.y - selecttolerance) <= mouse.y) {
                        if(key.selected)
                            key.selected = false
                        else
                            key.selected = true
                    }
                }
                mouseX = mouse.x
                mouseY = mouse.y
                track.requestPaint()
             }

            onPressed: {
                if(mouse.button == Qt.LeftButton){
                    movekey = false
                    moveframe = true
                }
                else if(mouse.button == Qt.RightButton){
                    movekey = true
                    moveframe = false
                }
                mouseX = mouse.x
                mouseY = mouse.y
            }

            onPositionChanged: {
                var selecttolerance = 4
                for(var i=0; i < curvemodel.count; i++){
                    var key = curvemodel.get(i)
                    if( (key.x + selecttolerance) >= mouse.x &&
                        (key.x - selecttolerance) <= mouse.x &&
                        (key.y + selecttolerance) >= mouse.y &&
                        (key.y - selecttolerance) <= mouse.y) {
                        key.hover = true
                    } else {
                        key.hover = false
                    }
                }
                if(moveframe){
                    var dX = mouseX - mouse.x
                    var dY = mouseY - mouse.y
                    minVal -= dY/10.0
                    maxVal -= dY/10.0
                    // Note - send back a signal to the timebar to update it's start and end times 
                    var length = (etime - stime)
                    var pps = width/length // pixels per second 
                    stime += dX/pps
                    etime += dX/pps
                    startTimeChanged(stime)
                    endTimeChanged(etime)
                    //console.log("y:",mouse.y,", min:",minVal,", max:",maxVal)
                    mouseX = mouse.x
                    mouseY = mouse.y
                 }
                else if(movekey){
                    var dX = parseFloat(mouseX - mouse.x)
                    var dY = parseFloat(mouseY - mouse.y)
                    var length = (etime - stime)
                    var pps = parseFloat(width/length)
                    var ppv = parseFloat(height/(maxVal - minVal))
                    for(var i=0; i < curvemodel.count; i++){
                        var key = curvemodel.get(i)
                        if(key.selected){
                            keyframe.uid = key.uid
                            var dVal = dY/ppv
                            // This is only for int values, add type check later
                            if(dVal >= 1 || dVal <= -1){
                                // get type
                                keyframe.fid = 4
                                if(keyframe.intVal == Field.Int){
                                    keyframe.fid = 2
                                    keyframe.intVal += dY/ppv
                                } else {
                                    keyframe.fid = 3
                                    keyframe.realVal += dY/ppv
                                }
                                mouseY = mouse.y
                            }
                            keyframe.fid = 1 // time
                            keyframe.realVal -= dX/pps
                         }
                    }
                }
                mouseX = mouse.x
                //mouseY = mouse.y
                track.requestPaint()
            }

            onReleased: {
                moveframe = false
                movekey = false
            }
        }
    }

    function draw_key(context,key) {
        keyframe.uid = key.uid
        keyframe.fid = 4
        var keytype = keyframe.intVal
        var keyfid = (keytype == Field.Int) ? 2 : 3 
        keyframe.fid = keyfid
        var val = (keytype == Field.Int) ? keyframe.intVal : keyframe.realVal
        keyframe.fid = 1
        var keyTime = keyframe.realVal
        var ppv = height/(maxVal - minVal)
        var length = (etime - stime)
        var pps = width/length // pixels per second 
        var keyX = (keyTime - stime) * pps
        var keyY = ppv*(maxVal-val)
        context.beginPath()
        context.lineWidth = 1

        if(key.hover)
            context.strokeStyle = "#00ff00"
        else
            context.strokeStyle = "#fb7e14"
        if(key.selected)
            context.strokeStyle = "#ff0000"

        context.rect(keyX-4,keyY-4,8,8)
        context.stroke()
        key.x = keyX
        key.y = keyY
    }

    function draw_curve(context) {
        if(curvemodel.count){
            context.beginPath()
            context.lineWidth = 1 
            context.strokeStyle = "#054552"
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

        // frame markers
        var sframe = Math.round(stime+100/spf)/100 // start frame
        var cpf = (sframe * ppf)-(stime * pps) // this is the start pixel for the drawn frame
        //console.log("cpf:",cpf," sframe:",sframe," spf:",spf," stime:",stime)
        var drawframe = false
        if((cpf/2)*2 == cpf)
            drawframe = true
        while(cpf < width) {
            //context.beginPath()
            context.fillStyle = "#aaaaaa"
            if(drawframe)
                context.fillRect(cpf,0,ppf,height)
            cpf += ppf
            drawframe = (drawframe) ? false : true
        }
        //context.stroke()

        // cpos 
        context.beginPath()
        context.strokeStyle = "#ff0000"
        context.lineWidth = ppf 
        context.moveTo(cposX,0)
        context.lineTo(cposX,height)
        context.stroke()

        while(secondX < width) {
            // draw lines
            context.beginPath()
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

        // Draw the horizontal lines
        // find 0 point
        var ppv = height/(maxVal-minVal) // pixels per value 1
        var zeroY = maxVal*ppv
        // zero marker
        context.beginPath()
        context.strokeStyle = "#ff00ff"
        context.lineWidth = 1 
        context.moveTo(0,zeroY)
        context.lineTo(width,zeroY)
        context.stroke()
        var ppvmult = 0 // pixel per value multiplier
        var ppvstep = ppv // pixel step to the next horizontal line
        while(ppvstep > 10 && ppvstep <= 100){
            ppvmult++
            ppvstep = ppv * ppvmult
        }
        var posvalmarkY = zeroY - ppvstep
        var negvalmarkY = zeroY + ppvstep
        context.strokeStyle = "#333333"

        // draw positive lines
        while(posvalmarkY > 0){
            context.beginPath()
            context.moveTo(0,posvalmarkY)
            context.lineTo(width,posvalmarkY)
            context.stroke()
            posvalmarkY = posvalmarkY - ppvstep
        }

        // draw negative lines
        while(negvalmarkY < height){
            context.beginPath()
            context.moveTo(0,negvalmarkY)
            context.lineTo(width,negvalmarkY)
            context.stroke()
            negvalmarkY = negvalmarkY + ppvstep
        }
 
    }

    // update the track keys
    function load_keys() {
        // get all the key uids
        curvemodel.clear()
        var tlist = SceneGraph.connected_uids(uid,2) 
        for(var i=0; tlist.length > i; i++){
            console.log("adding ",tlist[i]," to list")
            curvemodel.insert(i,{"uid":tlist[i],"x":0,"y":0,"selected":false,"hover":false})
        } 
    }

    function draw_keys() {

    }

    function update_track() {
        console.log("UPDATEING TRACK")
        load_keys()
        track.requestPaint()
    }

    function paint_track() {
        track.requestPaint()
    }
}
