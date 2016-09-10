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
        addFieldName("parent",1,201)
        addFieldName("child",1,202)

        // Camera 
        addFieldName("parent",2,201)
        addFieldName("child",2,202)
        addFieldName("type",2,1)
        addFieldName("fov",2,2)

        // Shape 
        addFieldName("parent",3,201)
        addFieldName("child",3,202)
        addFieldName("meshIn",3,1)
        addFieldName("meshOut",3,2)
 
        // Time 
        addFieldName("parent",4,201)
        addFieldName("child",4,202)
        addFieldName("stime",4,1)
        addFieldName("etime",4,2)
        addFieldName("cpos",4,3)
        addFieldName("fps",4,4)
        addFieldName("stime",4,5)
        addFieldName("etime",4,6)
        addFieldName("cpos",4,7)
        addFieldName("fps",4,8)
 
        // Multiply 
        addFieldName("parent",5,201)
        addFieldName("child",5,202)
        addFieldName("value",5,1)
        addFieldName("mult",5,2)
        addFieldName("value",5,3)
 
        // Divide 
        addFieldName("parent",6,201)
        addFieldName("child",6,202)
        addFieldName("value",6,1)
        addFieldName("div",6,2)
        addFieldName("value",6,3)

        // Transform
        addFieldName("parent",7,201)
        addFieldName("child",7,202)
        addFieldName("tX",7,1)
        addFieldName("tY",7,2)
        addFieldName("tZ",7,3)
        addFieldName("sX",7,4)
        addFieldName("sY",7,5)
        addFieldName("sZ",7,6)
        addFieldName("rX",7,7)
        addFieldName("rY",7,8)
        addFieldName("rZ",7,9)
        addFieldName("local_matrix",7,10)
        addFieldName("world_matrix",7,11)

        // Polygon Shape 
        addFieldName("parent",320,201)
        addFieldName("mesh",320,1)
        addFieldName("xform",320,2)
        // Out
        addFieldName("child",320,202)
        addFieldName("mesh",320,3)

        // Polygon Plane 
        addFieldName("parent",321,201)
        addFieldName("subX",321,1)
        addFieldName("subY",321,2)
        // Out
        addFieldName("child",321,202)
        addFieldName("mesh",321,3)

        // Polygon Cube
        // In
        addFieldName("parent",322,201)
        addFieldName("subX",322,1)
        addFieldName("subY",322,2)
        addFieldName("subZ",322,3)
        // Out 
        addFieldName("child",322,202)
        addFieldName("mesh",322,4)

        // ANIMATION PLUGIN
        // Int Key
        // In
        addFieldName("parent",420,201)
        addFieldName("time",420,1)
        addFieldName("value",420,2)
        addFieldName("in curve type",420,3)
        addFieldName("out curve type",420,4)
        // Out 
        addFieldName("child",420,202)
 
        // Real Key
        // In
        addFieldName("parent",421,201)
        addFieldName("time",421,1)
        addFieldName("value",421,2)
        addFieldName("in curve type",421,3)
        addFieldName("out curve type",421,4)
        // Out 
        addFieldName("child",421,202)
 
        // Int Key Track 
        // In
        addFieldName("parent",425,201)
        addFieldName("time",425,1)
        addFieldName("keys",425,2)
        // Out 
        addFieldName("child",425,202)
        addFieldName("value",425,3)
  
        // Real Key Track 
        // In
        addFieldName("parent",426,201)
        addFieldName("time",426,1)
        addFieldName("keys",426,2)
        // Out 
        addFieldName("child",426,202)
        addFieldName("value",426,3)

    }

}
