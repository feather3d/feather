/***********************************************************************
 *
 * Filename: Translation.qml 
 *
 * Description: This file holds all the translations for the interface. 
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

Item {
    ListModel {
        id: plugins

        // Empty
        ListElement { name: "Parent"; node: 1; field: 1 }
 
        // Polygon Shape 
        ListElement { name: "Parent"; node: 320; field: 1 }
        ListElement { name: "Mesh"; node: 320; field: 3 }
        ListElement { name: "Test"; node: 320; field: 4 }
        // Out
        ListElement { name: "Child"; node: 320; field: 2 }


        // Polygon Cube
        // In
        ListElement { name: "Parent"; node: 322; field: 1 }
        ListElement { name: "SubX"; node: 322; field: 3 }
        ListElement { name: "SubY"; node: 322; field: 4 }
        ListElement { name: "SubZ"; node: 322; field: 5 }
        // Out 
        ListElement { name: "Child"; node: 322; field: 2 }
        ListElement { name: "Mesh"; node: 322; field: 6 }

    }

    function get_field_name(nid,fid) {
        var index=0;
        while(index < plugins.count) {
            var obj = plugins.get(index);
            if(obj.node==nid && obj.field==fid)
                return obj.name;
            else
                index++;
        }
        return "ERROR"        
    }
}
