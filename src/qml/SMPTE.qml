/***********************************************************************
 *
 * Filename: SMPTE.qml 
 *
 * Description: Timecode information. 
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

Item {
    property double fps: 24
    property double position: 0 // in seconds
    property int hour: 0 
    property int minute: 0 
    property int second: 0 
    property int frame: 0 

    property string timecode: "00:00:00:00"

    signal changed()

    onPositionChanged: {
        hour=position/86400
        minute=(position - (position/86400))/60
        second=Math.floor(position)
        frame=(position - Math.floor(position)) * fps

        var hr
        var min
        var sec
        var frm

        hr = (hour < 10) ? "0" + hour : hour
        min = (minute < 10) ? "0" + minute : minute 
        sec = (second < 10) ? "0" + second : second
        frm = (frame < 10) ? "0" + frame : frame

        timecode = hr + ":" + min + ":" + sec + ":" + frm
        changed()
    }

    function set_hour(val) {
        hour=val
        build_text()
        changed()    
    }

    function set_minute(val) {
        minute=val
        build_text()
        changed()    
    }

    function set_second(val) {
        second=val
        build_text()
        changed()    
    }

    function set_frame(val) {
        frame=val
        build_text()
        changed()    
    }

    function set_timecode(msec){
        if(msec < 0){
            hour=0
            minute=0
            second=0
            frame=0
        } else {
            hour=msec/86400000
            minute=(msec - (msec/86400000))/60000
            second=(msec * 0.001).toFixed(0)
            frame=fps * (Math.abs(msec - ((msec*0.001).toFixed(0)*1000)) / 1000.00)
        }
        build_text()
        changed()
    }

    function build_text() {
        var hr
        var min
        var sec
        var frm

        hr = (hour < 10) ? "0" + hour : hour
        min = (minute < 10) ? "0" + minute : minute 
        sec = (second < 10) ? "0" + second : second
        frm = (frame < 10) ? "0" + frame : frame

        timecode = hr + ":" + min + ":" + sec + ":" + frm
    }
}
