/***********************************************************************
 *
 * Filename: TimeEditor.qml 
 *
 * Description: Controls time attributes of the scenegraph. 
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

import QtQuick 2.5
import feather.scenegraph 1.0
import feather.field 1.0

Rectangle {
    id: editor
    height: 50
    width: 100
    border.width: 1
    radius: 2
    color: "darkgrey"

    property int uid: 0 

    signal startTimeChanged(double time)
    signal endTimeChanged(double time)
    signal currentTimeChanged(double time)

    FieldValue {
        id: stime
        width: 50
        height: controller.height 
        anchors.top: parent.top
        anchors.left: parent.left
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 3
    }
    
    Rectangle {
        id: bar_box
        height: controller.height 
        anchors.left: stime.right
        anchors.top: parent.top
        anchors.right: etime.left
        border.width: 1
        radius: 2
        color: "orange"
       
        TimeBar {
            id: bar
            anchors.fill: parent
            anchors.margins: 2
            stime: slider.spos
            etime: slider.epos
       }
    }

    FieldValue {
        id: etime
        width: 50
        height: controller.height 
        anchors.top: parent.top
        anchors.right: controller.left
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 4
    }

    TimeController {
        id: controller
        anchors.top: parent.top
        anchors.right: parent.right
        cpos: cpos.realVal
        fps: fps.realVal
        //stime: stime.realValue
        //etime: etime.realValue
    }

    TimeSlider {
        id: slider
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: bar_box.bottom
        anchors.right: controller.left
        anchors.margins: 2
    }

    TimeCode {
        id: timecode
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: controller.bottom
        anchors.left: slider.right
        anchors.margins: 2
    }


    Field {
        id: cpos
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 5
    }

    Field {
        id: fps 
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 6
    }

    function setStartTime(time){
        slider.spos = time
    }

    function setEndTime(time){
        slider.epos = time
    }

    function setCurrentTime(time){
        cpos.realVal = time
    }

    function updateFields(uid,nid,fid) {
        stime.updateValue()
        etime.updateValue()
        slider.stime = stime.realValue
        slider.etime = etime.realValue
        controller.stime = stime.realValue
        controller.etime = etime.realValue
        bar.cpos = cpos.realVal
        bar.updateBar()
        timecode.pos = cpos.realVal
        startTimeChanged(stime.realValue)
        endTimeChanged(etime.realValue)
        currentTimeChanged(cpos.realValue)
    }

    function nodeSelected() {
        var uid = SceneGraph.selected_node();
        var nid = SceneGraph.node_id(uid);
        // if the nid is a animation track or an fid that connects to a track, set the track_uid in the bar so the keys can be displayed
        // TODO - add fid check
        if ( nid == 425 || nid == 426 ) {
            bar.track_uid = uid
        }
    }

    function updatePosition(pos) {
        console.log("new pos: " + cpos.realVal)
        cpos.realVal = pos
        //timecode.pos = cpos.realVal
    }

    function updateCPos() {
        console.log("updating cpos")
        timecode.pos = cpos.realVal
        bar.cpos = cpos.realVal
        bar.updateBar()
        controller.cpos = cpos.realVal
        currentTimeChanged(cpos.realVal)
    }

    function barCposChanged() {
        cpos.realVal = bar.cpos
    }

    Component.onCompleted: {
        slider.stime = stime.realValue
        slider.etime = etime.realValue
        bar.cpos = cpos.realVal
        bar.cposChanged.connect(barCposChanged)
        controller.cpos = cpos.realVal
        controller.stime = stime.realValue
        controller.etime = etime.realValue
        controller.positionChanged.connect(updatePosition) 
        SceneGraph.nodeFieldChanged.connect(updateFields)
        SceneGraph.nodeSelected.connect(nodeSelected)
        cpos.realValChanged.connect(updateCPos)
    }
}
