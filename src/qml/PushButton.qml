/***********************************************************************
 *
 * Filename: PushButton.qml 
 *
 * Description: A standard button used for dialog boxes and other misc items.
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
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "common"

Button {
    id: button
    width: 100
    height: 20
    property Properties properties: Null
 
    style: ButtonStyle { 
        Rectangle {
            id: styleFrame
            implicitWidth: 100
            implicitHeight: 20
            border.width: 1
            color: properties.getColor("buttonEnabledBg") 
            radius: 2
        }
    }
}
