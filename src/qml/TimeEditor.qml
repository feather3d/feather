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
    property alias stime: stimefield.realValue
    property alias etime: etimefield.realValue
    property alias cpos: cposfield.realVal
    property alias spos: slider.spos // slider start time
    property alias epos: slider.epos // slider end time

    signal startTimeChanged(double time)
    signal endTimeChanged(double time)
    signal currentTimeChanged(double time)
    signal sliderStartPositionChanged(double time)
    signal sliderEndPositionChanged(double time)
 
    FieldValue {
        id: stimefield
        width: 50
        height: controller.height 
        anchors.top: parent.top
        anchors.left: parent.left
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 1
    }
    
    Rectangle {
        id: bar_box
        height: controller.height 
        anchors.left: stimefield.right
        anchors.top: parent.top
        anchors.right: etimefield.left
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
        id: etimefield
        width: 50
        height: controller.height 
        anchors.top: parent.top
        anchors.right: controller.left
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 2
    }

    TimeController {
        id: controller
        anchors.top: parent.top
        anchors.right: parent.right
        cpos: cposfield.realVal
        fps: fps.realVal
        //stime: stimefield.realValue
        //etime: etimefield.realValue
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
        id: cposfield
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 3
    }

    Field {
        id: fps 
        uid: SceneGraph.get_node_by_name("time")
        nid: 4
        fid: 4
    }

    function setStartTime(time){
        // don't allow the slide to go under the set start time
        if(stimefield.realValue <= time && slider.epos <= etimefield.realValue)
            slider.spos = time
    }

    function setEndTime(time){
        // don't allow the slide to go under the set start time
        if(etimefield.realValue >= time && slider.spos >= stimefield.realValue)
            slider.epos = time
    }

    function setCurrentTime(time){
        cposfield.realVal = time
    }

    function updateFields(uid,nid,fid) {
        stimefield.updateValue()
        etimefield.updateValue()
        slider.stime = stimefield.realValue
        slider.etime = etimefield.realValue
        controller.stime = stimefield.realValue
        controller.etime = etimefield.realValue
        bar.cpos = cposfield.realVal
        bar.updateBar()
        timecode.pos = cposfield.realVal
        startTimeChanged(stimefield.realValue)
        endTimeChanged(etimefield.realValue)
        currentTimeChanged(cposfield.realVal)
    }

    function nodeSelected() {
        var uid = SceneGraph.selected_node();
        var nid = SceneGraph.node_id(uid);
        // if the nid is a animation track or an fid that connects to a track, set the track_uid in the bar so the keys can be displayed
        // TODO - add fid check
        if (nid == 425)
            bar.track_uid = uid
    }

    function updatePosition(pos) {
        console.log("new pos: " + cposfield.realVal)
        cposfield.realVal = pos
        //timecode.pos = cposfield.realVal
    }

    function updateCPos() {
        console.log("updating cpos")
        timecode.pos = cposfield.realVal
        bar.cpos = cposfield.realVal
        bar.updateBar()
        controller.cpos = cposfield.realVal
        currentTimeChanged(cposfield.realVal)
    }

    function barCposChanged() {
        cposfield.realVal = bar.cpos
        SceneGraph.nodeFieldChanged(cposfield.uid,cposfield.nid,cposfield.fid)
        //SceneGraph.triggerUpdate()
    }

    function sliderChanged() {
        sliderStartPositionChanged(slider.spos)
        sliderEndPositionChanged(slider.epos)
    }

    Component.onCompleted: {
        slider.barChanged.connect(sliderChanged)
        slider.stime = stimefield.realValue
        slider.etime = etimefield.realValue
        bar.cpos = cposfield.realVal
        bar.cposChanged.connect(barCposChanged)
        controller.cpos = cposfield.realVal
        controller.stime = stimefield.realValue
        controller.etime = etimefield.realValue
        controller.positionChanged.connect(updatePosition) 
        SceneGraph.nodeFieldChanged.connect(updateFields)
        SceneGraph.nodeSelected.connect(nodeSelected)
        cposfield.realValChanged.connect(updateCPos)
    }
}
