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
    property alias uid: keyframes.uid // track node uid
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

    Field { id: keyframes; nid: 420; fid: 2 }

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

            // draw curve
            draw_curve(context)
 
            // draw keys
            if(uid) {
                for(var i=0; i < curvemodel.count; i++)
                    draw_key(context,curvemodel.get(i))
            }

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
                else if(movekey) {
                    var dX = parseFloat(mouseX - mouse.x)
                    var dY = parseFloat(mouseY - mouse.y)
                    var length = (etime - stime)
                    var pps = parseFloat(width/length)
                    var ppv = parseFloat(height/(maxVal - minVal))
                    for(var i=0; i < curvemodel.count; i++){
                        var key = curvemodel.get(i)
                        //var key = keyframes.key(keydata.i)
                        // TODO - FIX THE BELOW
                        if(key.selected){
                            var dVal = dY/ppv
                            if(dVal >= 1 || dVal <= -1){
                                // set value
                                key.key.value += dY/ppv
                                key.key.cp1y += dY/ppv
                                key.key.cp2y += dY/ppv
                                mouseY = mouse.y
                            }
                            // set time
                            key.key.time -= dX/pps
                            if(curvemodel.get(i-1).key.time > key.key.time)
                                key.key.time = curvemodel.get(i-1).key.time + (1.0/24)
                            if(curvemodel.count > (i+1) ){
                                if(curvemodel.get(i+1).key.time < key.key.time)
                                    key.key.time = curvemodel.get(i+1).key.time - (1.0/24)
                                if(curvemodel.get(i+1).key.cp1x < key.key.time)
                                    curvemodel.get(i+1).key.cp1x = key.key.time
                                if(curvemodel.get(i+1).key.time < key.key.cp2x)
                                    key.key.cp2x = curvemodel.get(i+1).key.time
                            } 
                            if(curvemodel.get(i-1).key.cp2x > key.key.time)
                                curvemodel.get(i-1).key.cp2x = key.key.time
                           
                            key.key.cp1x -= dX/pps
                            if( i > 0 )
                                if(curvemodel.get(i-1).key.time > key.key.cp1x)
                                    key.key.cp1x = curvemodel.get(i-1).key.time
                            key.key.cp2x -= dX/pps
                            keyframes.setKeyArrayValue(key.index,key.key.time,key.key.value,key.key.inCurve,key.key.cp1x,key.key.cp1y,key.key.outCurve,key.key.cp2x,key.key.cp2y)
                         }
                        
                    }
                }
                mouseX = mouse.x
                track.requestPaint()
            }

            onReleased: {
                moveframe = false
                movekey = false
            }
        }
    }

    function draw_key(context,key) {
        var ppv = height/(maxVal - minVal)
        var length = (etime - stime)
        var pps = width/length // pixels per second 
        var keyX = (key.key.time - stime) * pps
        var keyY = ppv*(maxVal-key.key.value)

        // draw the in and out weights if a bezier or quadtratic curve
        // for testing we'll just make dummys for now
        
        // draw control points
        context.beginPath()
        context.lineWidth = 1
        // cp1
        var cp1X = (key.key.cp1x - stime) * pps
        var cp1Y = ppv*(maxVal - key.key.cp1y)
        context.strokeStyle = "#0000ff"
        context.rect(cp1X-4,cp1Y-4,8,8)
        // cp2
        var cp2X = (key.key.cp2x - stime) * pps
        var cp2Y = ppv*(maxVal - key.key.cp2y)
        context.strokeStyle = "#0000ff"
        context.rect(cp2X-4,cp2Y-4,8,8)
        context.stroke()


        // draw lines between control points and key
        context.beginPath()
        context.lineWidth = 2
        context.strokeStyle = "#00ff00"
        context.moveTo(cp1X,cp1Y)
        context.lineTo(keyX,keyY)
        context.lineTo(cp2X,cp2Y)
        context.stroke()
 
        context.beginPath()
        context.lineWidth = 2

        // draw the keyframe point
        if(key.hover)
            context.fillStyle = "#00ff00"
        else
            context.fillStyle = "#fb7e14"
        if(key.selected)
            context.fillStyle = "#ff0000"

        context.ellipse(keyX-5,keyY-5,10,10)
        context.fill()
        key.x = keyX
        key.y = keyY

       

    }

    function draw_curve(context) {
        if(curvemodel.count > 1){
            var ppv = height/(maxVal - minVal)
            var length = (etime - stime)
            var pps = width/length // pixels per second 
 

            var sx = (curvemodel.get(0).key.time - stime) * pps
            var sy = ppv*(maxVal - curvemodel.get(0).key.value)
 
            context.lineWidth = 2 
            context.strokeStyle = "#054552"
            context.beginPath()
            context.moveTo(sx, sy)
            //console.log("draw_curve() - sx:",sx," sy:",sy)
            for(var i=0; i < (curvemodel.count-1); i++){
                var p1cp2X = (curvemodel.get(i).key.cp2x - stime) * pps
                var p1cp2Y = ppv*(maxVal - curvemodel.get(i).key.cp2y)
                var p2cp1X = (curvemodel.get(i+1).key.cp1x - stime) * pps
                var p2cp1Y = ppv*(maxVal - curvemodel.get(i+1).key.cp1y)
                var p2X = (curvemodel.get(i+1).key.time - stime) * pps
                var p2Y = ppv*(maxVal - curvemodel.get(i+1).key.value)
                context.bezierCurveTo(p1cp2X, p1cp2Y, p2cp1X, p2cp1Y, p2X, p2Y)
                //console.log("p1 cp2X:",p1cp2X," p1 cp2Y:",p1cp2Y," p2 cp1X:",p2cp1X," p2 cp1Y:",p2cp1Y," p2 X:",p2X," p2 Y:",p2Y)
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
        var cframe = Math.round(cpos*fps)
        var sframe = Math.round(stime*fps)
        var cposX = (cframe- sframe) * ppf
        var cframe = Math.floor(cpos*fps)
        var csec = Math.floor(stime)

        // frame markers
        var drawframe = false

        var sframe = Math.floor(fps * stime)
        var cpf =  ((sframe * spf) - stime) * pps
 
        //console.log("cpf:",cpf," sframe:",sframe," stime:",stime)
 
        if((Math.floor(sframe/2)*2) == sframe) 
            drawframe = true

        context.beginPath()
        context.lineWidth = ppf
        while(cpf < width) {
            context.strokeStyle = "#aaaaaa"
            //context.fillStyle = "#aaaaaa"
            if(drawframe){
                context.moveTo(cpf+(ppf/2),0)
                context.lineTo(cpf+(ppf/2),height)
            }                
            //context.fillRect(cpf,0,ppf,height)
            cpf += ppf
            drawframe = (drawframe) ? false : true
        }
        context.stroke()
 
        // cpos 
        context.beginPath()
        context.strokeStyle = "#ff0000"
        context.lineWidth = ppf 
        context.moveTo(cposX,0)
        context.lineTo(cposX,height)
        context.stroke()

            
        context.beginPath()
        while(secondX < width) {
            // draw lines
            context.strokeStyle = "#000000"
            context.lineWidth = 1
            context.moveTo(secondX,0)
            context.lineTo(secondX,height)
            // display the frame number
            context.fillStyle = "#000000"
            context.fillText(csec,secondX,height-10)
            secondX = secondX + pps
            csec += 1
        }
        context.stroke()

        // Draw the horizontal lines
        var ppv = height/(maxVal-minVal) // pixels per value 1
        var cpv = (maxVal - Math.floor(maxVal)) * ppv
        var cv = Math.floor(maxVal)

        while(cpv <= height){
            if(cv != 0){
                context.beginPath()
                context.strokeStyle = "#333333"
                context.lineWidth = 1 
                context.moveTo(0,cpv)
                context.lineTo(width,cpv)
                context.fillStyle = "#000000"
                context.fillText(cv,2,cpv+(ppv/2))
                context.stroke()
             } else {
                //console.log("draw 0")
                context.beginPath()
                context.strokeStyle = "#ff00ff"
                context.lineWidth = 1 
                context.moveTo(0,cpv)
                context.lineTo(width,cpv)
                context.stroke()
            }
            cv -= 1
            cpv += ppv 
        }

    }

    // update the track keys
    function load_keys() {
        // get all the key uids
        //console.log("loading keys")
        curvemodel.clear()
        //keyframes.uid = uid
        keyframes.nid = SceneGraph.node_id(uid)
        // verify that we are looking at a track node
        if ( keyframes.nid != 420 ) {
            //console.log("The keyframes field is not a KeyTrack node")
            //console.log("\tuid:",keyframes.uid," nid:",keyframes.nid," fid:",keyframes.fid)
            return
        } 
        var tlist = keyframes.keyArrayVal 
        //console.log("LOADING TRACK NODE uid:",keyframes.uid," nid:",keyframes.nid," fid:",keyframes.fid)
        for(var i=0; i < tlist.length; i++){
            //console.log("adding key time:",tlist[i].time," value:",tlist[i].value," to list")
            //curvemodel.insert(i,{"key":tlist[i],"index":i,"time":tlist[i].time,"value":tlist[i].value,"x":0,"y":0,"selected":false,"hover":false})
            curvemodel.insert(i,{"key":tlist[i],"index":i,"x":0,"y":0,"selected":false,"hover":false})
        } 
    }

    function draw_keys() {

    }

    function update_track() {
        //console.log("UPDATING TRACK")
        load_keys()
        track.requestPaint()
    }

    function paint_track() {
        track.requestPaint()
    }

    function updateTrack(uid,nid,fid) {
        //console.log("update key track uid:",uid," nid:",nid," fid:",fid)
        if ( nid == 420 ) {
            frame.uid = uid
        } else {
            // get the track from the node field
            var uids = SceneGraph.connected_uids(uid,fid)
            for ( var i=0; i < uids.length; i++ ) {
                var cnid = SceneGraph.node_id(uids[i])
                if ( cnid == 420 ) {
                    frame.uid = uids[i]
                    keyframes.nid = 420
                    update_track()
                }
            }
        }
    }

    Component.onCompleted: {
        SceneGraph.keyAdded.connect(updateTrack)
    }
}
