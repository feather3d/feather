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
        // Base 
        // in
        addFieldName("parent",0,201)
        addFieldName("tx",0,203)
        addFieldName("ty",0,204)
        addFieldName("tz",0,205)
        addFieldName("rx",0,206)
        addFieldName("ry",0,207)
        addFieldName("rz",0,208)
        addFieldName("sx",0,209)
        addFieldName("sy",0,210)
        addFieldName("sz",0,211)
 
        // out
        addFieldName("child",0,202)
        addFieldName("local",0,212)
        addFieldName("world",0,213)

        // Empty
        // no fields

        // Camera 
        addFieldName("type",2,1)
        addFieldName("fov",2,2)

        // Shape
        // in 
        addFieldName("mesh",3,1)
        // out
        addFieldName("mesh",3,2)
 
        // Time 
        addFieldName("stime",4,1)
        addFieldName("etime",4,2)
        addFieldName("cpos",4,3)
        addFieldName("fps",4,4)
        addFieldName("stime",4,5)
        addFieldName("etime",4,6)
        addFieldName("cpos",4,7)
        addFieldName("fps",4,8)
 
        // Multiply 
        addFieldName("value",5,1)
        addFieldName("mult",5,2)
        addFieldName("value",5,3)
 
        // Divide 
        addFieldName("parent",6,201)
        addFieldName("child",6,202)
        addFieldName("value",6,1)
        addFieldName("div",6,2)
        addFieldName("value",6,3)

        // Polygon Shape 
        addFieldName("mesh",320,1)
        addFieldName("xform",320,2)
        // Out
        addFieldName("mesh",320,3)

        // Polygon Plane 
        addFieldName("subX",321,1)
        addFieldName("subY",321,2)
        // Out
        addFieldName("mesh",321,3)

        // Polygon Cube
        // In
        addFieldName("subX",322,1)
        addFieldName("subY",322,2)
        addFieldName("subZ",322,3)
        // Out 
        addFieldName("mesh",322,4)

        // ANIMATION PLUGIN
        // Int Key
        // In
        addFieldName("time",420,1)
        addFieldName("value",420,2)
        addFieldName("in curve type",420,3)
        addFieldName("out curve type",420,4)
 
        // Real Key
        // In
        addFieldName("time",421,1)
        addFieldName("value",421,2)
        addFieldName("in curve type",421,3)
        addFieldName("out curve type",421,4)
 
        // Int Key Track 
        // In
        addFieldName("time",425,1)
        addFieldName("keys",425,2)
        // Out 
        addFieldName("value",425,3)
  
        // Real Key Track 
        // In
        addFieldName("time",426,1)
        addFieldName("keys",426,2)
        // Out 
        addFieldName("value",426,3)
    }

}
