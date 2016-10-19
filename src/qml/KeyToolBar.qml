/***********************************************************************
 *
 * Filename: KeyToolBar.qml 
 *
 * Description: Toolbar options for a selected key. 
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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import feather.scenegraph 1.0
import feather.field 1.0

Rectangle {
    id: toolbar 
    color: "grey"

    height: 28 
    width: 200
 
    Action {
        id: addKeyAct
        text: "Add Key"
        tooltip: "add key at current time"
        onTriggered: { }
    }
 
    Action {
        id: removeKeyAct
        text: "Remove Key"
        tooltip: "remove key at current time"
        onTriggered: { }
    }
 
    Action {
        id: linearCurveAct
        text: "Linear Curve"
        tooltip: "change selected keys in/out curves to linear type"
        onTriggered: { }
    }
 
    Action {
        id: bezierCurveAct
        text: "Bezier Key"
        tooltip: "change selected keys in/out curves to bezier type"
        onTriggered: { }
    }

    Action {
        id: flattenCpAct
        text: "Flatten Cp"
        tooltip: "flatten the keyframe's control point's value"
        onTriggered: { }
    }

    Action {
        id: lockCpAct
        text: "Lock Cp"
        tooltip: "lock the control point's so one will move with the other"
        onTriggered: { }
    }



    RowLayout {
        spacing: 2

        ToolButton {
            id: addKeyBtn
            width: 20
            height: 20
            iconSource: "icons/add_key.svg"
            action: addKeyAct
        }

        ToolButton {
            id: removeKeyBtn
            width: 20
            height: 20
            iconSource: "icons/remove_key.svg"
            action: removeKeyAct
        }

        ToolButton {
            id: linearCurveBtn
            width: 20
            height: 20
            iconSource: "icons/linear_curve.svg"
            action: linearCurveAct
         }

        ToolButton {
            id: bezierCurveBtn
            width: 20
            height: 20
            iconSource: "icons/bezier_curve.svg"
            action: bezierCurveAct
         }

        ToolButton {
            id: flattenCpBtn
            width: 20
            height: 20
            iconSource: "icons/bezier_curve_flatten_cp.svg"
            action: flattenCpAct
         }

        ToolButton {
            id: lockCpBtn
            width: 20
            height: 20
            iconSource: "icons/bezier_curve_lock_cp.svg"
            action: lockCpAct
         }

    }
} 
