/***********************************************************************
 *
 * Filename: PopupMenu.qml 
 *
 * Description: Template Popup menu used by various editors popup menus. 
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

Rectangle {
    id: popup
    width: 100
    height: 200
    visible: false

    border.color: "black"
    border.width: 1
    color: "grey"

    property alias model: view.model 

    signal connectionButtonPressed(int button, int uid, int nid, int fid)
    signal connectionButtonReleased(int button, int uid, int nid, int fid)
    signal connectionButtonClicked(int button, int uid, int nid, int fid)

    Translation { id: translation }
   
    ListView {
        id: view
        anchors.fill: parent
        anchors.margins: 1
        delegate: MenuButton {
            label: translation.get_field_name(nid,fid)
            ftype: type
            sgId: uid
            nodeId: nid
            fieldId: fid
            Component.onCompleted: {
                connectionButtonPressed.connect(doConnectionPressed)
                connectionButtonReleased.connect(doConnectionReleased)
                connectionButtonClicked.connect(doConnectionClicked)
            }
        } 
        spacing: 1

    }

    function resize() {
        popup.height = (popup.model.rowCount() * 15) + popup.model.rowCount();
    }

    function doConnectionPressed(button,uid,nid,fid) {
        popup.visible=false
        connectionButtonPressed(button,uid,nid,fid)
    }

    function doConnectionReleased(button,uid,nid,fid) {
        // mouse button release not used yet
        //connectionButtonReleased(button,uid,nid,fid)
    }

    function doConnectionClicked(button,uid,nid,fid) {
        //popup.visible=false
        //connectionButtonClicked(button,uid,nid,fid)
    }
}
