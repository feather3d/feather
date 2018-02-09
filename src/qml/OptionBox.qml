/***********************************************************************
 *
 * Filename: OptionBox.qml 
 *
 * Description: Feather version of the Qt ComboBox.
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

ComboBox {
    id: combobox
    width: 100
    height: 30
    property Properties properties: Null

    style: ComboBoxStyle {
        id: comboStyle

        background: Rectangle {
            id: bgFrame
            color: properties.getColor("optionSelectedBg") 
            border.color: "black"
            border.width: 1
            radius: 2
        }

        label: Text {
                id: optionLabel
                
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 14
                font.bold: false
                color: properties.getColor("optionSelectedText")
                text: currentText
        }

        dropDownButtonWidth: 30
        selectionColor: properties.getColor("optionSelectedBg") 
        selectedTextColor: properties.getColor("optionSelectedText") 
    }
}
