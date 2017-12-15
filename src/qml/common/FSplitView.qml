/***********************************************************************
 *
 * Filename: FSplitView.qml 
 *
 * Description: Feather custom split view widget. 
 *
 * Copyright (C) 2017 Richard Layman, rlayman2000@yahoo.com 
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

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

SplitView {
    id: view
    anchors.fill: parent
    orientation: Qt.Horizontal
    resizing: true

    FTheme { id: theme }

    handleDelegate: Rectangle {
        height: theme.splitViewHandleWidth
        width: theme.splitViewHandleWidth
        color: (styleData.hovered) ? ((styleData.pressed) ? theme.splitViewHandleBackgroundActiveColor :  theme.splitViewHandleBackgroundPressedColor) : theme.splitViewHandleBackgroundNormalColor
        border.color: theme.splitViewHandleBorderColor
        border.width: theme.splitViewHandleBorderWidth
        radius: theme.splitViewHandleBorderRadius
    }
}


