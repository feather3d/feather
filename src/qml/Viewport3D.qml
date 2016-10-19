/***********************************************************************
 *
 * Filename: Viewport3D.qml 
 *
 * Description: Holds the C++ viewport. 
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

import QtQuick 2.3
import QtQuick.Scene3D 2.0
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0
import Qt3D.Input 2.0
import feather.viewport 1.0
import feather.scenegraph 1.0
import feather.outliner 1.0

Rectangle {
    id: frame
    color: "yellow"
    property alias showGrid: vp.showGrid
    property alias showAxis: vp.showAxis
    property alias majorSubDividLevel: vp.majorSubDividLevel
    property alias minorSubDividLevel: vp.minorSubDividLevel

    OutlinerModel { id: treeModel }

    Scene3D {
        id: scene3d
        anchors.fill: parent
        anchors.margins: 2
        //focus: true
        aspects: ["input","logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Viewport3D2 { id: vp }

    } //sceneRoot

    onWidthChanged: { vp.width = width }

    onHeightChanged: { vp.height = height }

    function addNode(uid) {
        console.log("vp.addNode(",uid,")")
        vp.addItems(uid) 
    }

    function removeNode(uid) {
        console.log("vp.removeNode(",uid,")")
        vp.removeItem(uid) 
    }

    function addDrawItems(item) {
        console.log("vp.addDrawItems(",item,")")
        vp.addItems(item)
    }

    function updateDrawItems(uid) {
        vp.updateItems(uid)
    }

    function updateViewport(uid,nid,fid) {
        console.log("UPDATE THE VIEWPORT!!!!")
        vp.doUpdate()
    }

    function graphUpdated() {
        console.log("UPDATE THE VIEWPORT!!!!")
        vp.doUpdate()
    }

    function clear() {
        vp.clearDrawItems();
    }

    Component.onCompleted: {
        SceneGraph.nodeAdded.connect(addNode)
        SceneGraph.nodeAddDrawItems.connect(addDrawItems)
        SceneGraph.nodeUpdateDrawItems.connect(updateDrawItems)
        SceneGraph.nodeFieldChanged.connect(updateViewport)
        SceneGraph.updateGraph.connect(graphUpdated)
        SceneGraph.nodeRemoved.connect(removeNode)
        SceneGraph.cleared.connect(clear)
    }
}
