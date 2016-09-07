/***********************************************************************
 *
 * Filename: KeyframeEditor.qml 
 *
 * Description: Editor for keyframe values and curves. 
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

Rectangle {
    id: editor
    color: "darkgrey"
    anchors.fill: parent
    property alias uid: track.uid // track node uid
    property alias stime: track.stime // seconds
    property alias etime: track.etime // seconds
    property alias fps: track.fps 
    property alias cpos: track.cpos //seconds

    signal startTimeChanged(double time)
    signal endTimeChanged(double time)
    signal currentTimeChanged(double time)


    ListModel {
        id: trackmodel

        ListElement {
            name: "test_track"
            uid: 0
        }
    }

    SplitView {
        id: view
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle {
            id: names
            width: 200
            color: "darkgrey"
            border.color: "black"
            border.width: 1

            ListView {
                anchors.fill: parent
                anchors.margins: 2 
                model: trackmodel
                delegate: Text { text: name }
            } 
        }

        KeyTrack {
            id: track
            anchors.left: names.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
    }

    function nodeSelect() {
        var suid = SceneGraph.selected_node()
        var snid = SceneGraph.node_id(suid)
        console.log("TRACK UID:",suid," NID:",snid)
        if(snid == 425 || snid == 426){
            uid = suid
            trackmodel.clear()
            trackmodel.insert(0,{"name": SceneGraph.node_name(uid), "uid": uid })
            track.update_track()
        }
    }

    function setStartTime(time) {
        track.stime = time
    }

    function setEndTime(time) {
        track.etime = time
    }

    function setCurrentTime(time) {
        track.cpos = time
    }

    Component.onCompleted: {
        SceneGraph.nodeSelected.connect(nodeSelect)
        track.paint_track()
        track.startTimeChanged.connect(startTimeChanged)
        track.endTimeChanged.connect(endTimeChanged)
        track.currentTimeChanged.connect(currentTimeChanged)
    }
}
