/***********************************************************************
 *
 * Filename: OutlinerLeafNode.qml 
 *
 * Description: Leaf node drawn by the Outliner. 
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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TreeViewStyle {
    id: style

    backgroundColor: "#848484"
    alternateBackgroundColor: "#949494"

    itemDelegate:  Rectangle {
        color: (styleData.selected)?"#7070FF":"#00000000"

        Item {
            id: nodeName
            visible: false
            
            Row {
                Image {
                    sourceSize.width: 18
                    sourceSize.height: 18
                    source: "icons/"+model.icon
                }
            Text {
                //elide: styleData.elideMode
                font.pixelSize: 12
                text: model.name
            }
        }

        }

        Item { 
            id: nodeVisible
            visible: false
            width: 20
            height: 20
            Image {
                id: visible_icon
                visible: (model.visible) ? true : false
                sourceSize.width: 18
                sourceSize.height: 18
                source: "icons/visible.svg"
            }

            Image {
                id: not_visible_icon
                visible: (model.visible) ? false : true 
                sourceSize.width: 18
                sourceSize.height: 18
                source: "icons/not_visible.svg"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    /*
                    visible_icon.visible = (visible_icon.visible) ? false : true
                    not_visible_icon.visible = (not_visible_icon.visible) ? false : true
                    */
                }
            }
        }
        //Item { id: nodeVisible; visible: false; CheckBox { checked: model.visible } }

        function updateVisiblity(uid,state){
            console.log("updateVisiblity() reached")
            // TODO - latter I may add a check here to save a little time
            visible_icon.visible = (model.visible) ? false : true
            not_visible_icon.visible = (model.visible) ? true : false 
        }

        Component.onCompleted: {
            //model.visiblityChanged.connect(updateVisiblity)
            switch(styleData.column){
                case 0:
                    nodeName.visible=true;
                    nodeVisible.visible=false;
                    break;
                case 1:
                    nodeName.visible=false;
                    nodeVisible.visible=true;
                    break;
                default:
                    0 
            }
        }
    }

}

