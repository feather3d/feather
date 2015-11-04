/***********************************************************************
 *
 * Filename: NodeFields.qml 
 *
 * Description: This file holds all the field names for the nodes. 
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

import QtQuick 2.1
import feather.field 1.0

FieldModel {

    Component.onCompleted: {
        // Empty
        addFieldName("Parent",1,1)
 
        // Polygon Shape 
        addFieldName("Parent",320,1)
        addFieldName("Mesh",320,3)
        addFieldName("Test",320,4)
        // Out
        addFieldName("Child",320,2)


        // Polygon Cube
        // In
        addFieldName("Parent",322,1)
        addFieldName("SubX",322,3)
        addFieldName("SubY",322,4)
        addFieldName("SubZ",322,5)
        // Out 
        addFieldName("Child",322,2)
        addFieldName("Mesh",322,6)
    }

}
