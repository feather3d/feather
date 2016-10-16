/***********************************************************************
 *
 * Filename: TimeController.qml 
 *
 * Description: Controls playback. 
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
import feather.command 1.0
import feather.scenegraph 1.0


Rectangle {
    id: window
    width: 148 
    height: 28 
    color: "lightgrey"
    border.width: 1
    property double cpos: 0
    property double stime: 0
    property double etime: 10
    property double fps: 24
    //property alias smpte: time.smpte

    signal play_forward()
    signal play_backward()
    signal seek(double pos)
    signal stop()
    signal pause()
    signal positionChanged(double pos)

    ExclusiveGroup { id: group }

    Timer {
        id: timer
        repeat: true
        running: false
        interval: 1000/fps
    }

    Item {
        id: controller
        anchors.fill: parent
        anchors.margins: 4

        Row {
            spacing: 4

            ImageButton {
                id: addKeyButton 
                width: 20
                height: 20
                imageNormal: "icons/add_key.svg"
                imageActive: "icons/add_key.svg"
                imageHover: "icons/add_key.svg"
                imageDisabled: "icons/add_key.svg"
                //exclusiveGroup: group
            }

            ImageButton {
                id: rewindButton 
                width: 20
                height: 20
                imageNormal: "icons/button_rewind_normal.svg"
                imageActive: "icons/button_rewind_active.svg"
                imageHover: "icons/button_rewind_hover.svg"
                imageDisabled: "icons/button_rewind_disabled.svg"
                exclusiveGroup: group
            }

            ImageButton {
                id: backFrameButton 
                width: 20 
                height: 20 
                imageNormal: "icons/button_frame_backward_normal.svg"
                imageActive: "icons/button_frame_backward_active.svg"
                imageHover: "icons/button_frame_backward_hover.svg"
                imageDisabled: "icons/button_frame_backward_disabled.svg"
                exclusiveGroup: group
            }

            ImageButton {
                id: playBackwardButton 
                width: 20
                height: 20
                imageNormal: "icons/button_play_backward_normal.svg"
                imageActive: "icons/button_play_backward_active.svg"
                imageHover: "icons/button_play_backward_hover.svg"
                imageDisabled: "icons/button_play_backward_disabled.svg"
                exclusiveGroup: group
            }

            ImageButton {
                id: playForwardButton 
                width: 20
                height: 20
                imageNormal: "icons/button_play_forward_normal.svg"
                imageActive: "icons/button_play_forward_active.svg"
                imageHover: "icons/button_play_forward_hover.svg"
                imageDisabled: "icons/button_play_forward_disabled.svg"
                exclusiveGroup: group
             }

            ImageButton {
                id: forwardFrameButton 
                width: 20
                height: 20
                imageNormal: "icons/button_frame_forward_normal.svg"
                imageActive: "icons/button_frame_forward_active.svg"
                imageHover: "icons/button_frame_forward_hover.svg"
                imageDisabled: "icons/button_frame_forward_disabled.svg"
                exclusiveGroup: group
             }

            ImageButton {
                id: fastForwardButton 
                width: 20
                height: 20
                imageNormal: "icons/button_fast_forward_normal.svg"
                imageActive: "icons/button_fast_forward_active.svg"
                imageHover: "icons/button_fast_forward_hover.svg"
                imageDisabled: "icons/button_fast_forward_disabled.svg"
                exclusiveGroup: group
             }
            /* 
            TimeCode {
                id: time
                width: 80
                height: 20
                timeSize: 12
                timeColor: "yellow"
            }
            */
        }
    }

    onCposChanged: {
        positionChanged(cpos)
        console.log("CPOS CHANGED TO:" + cpos) 
    } 


    // COMMANDS

    Command {
        id: addKey 
        name: "add_key"
        parameters: [
            Parameter { 
                id: addKeyTime
                name: "time"
                type: Parameter.Real
                realValue: 0.0 
            },

            Parameter { 
                id: addKeyValue
                name: "value"
                type: Parameter.Real
                realValue: 0.0 
            },

            Parameter { 
                id: addKeyFid
                name: "fid"
                type: Parameter.Int
                intValue: 0.0 
            },

            Parameter { 
                id: addKeyUid 
                name: "uid"
                type: Parameter.Int
                intValue: 0
            }
        ]
    }


    // FUNCTIONS


    function on_add_key(){
        // TODO - this is disabled until the addKey feature is fixed
       //addKey.exec() 
       SceneGraph.triggerUpdate()
    }

    function on_play_forward(){
        if(!playForwardButton.checked) {
            playForwardButton.checked=true
            timer.start()
            play_forward()
        } else {
            playForwardButton.checked=false
            timer.stop()
            stop()
        }
    }

    function on_play_backward(){
        if(!playBackwardButton.checked) {
            playBackwardButton.checked=true
            timer.start()
            play_backward()
        } else {
            playBackwardButton.checked=false
            timer.stop()
            stop()
        }
    }

    function update_play() {
        if(playForwardButton.checked) {
            var tpos = cpos + (1/fps)
            if(tpos > etime) {
                timer.stop()
                cpos = etime     
            } else {
                cpos = tpos
            }
        }

        else if(playBackwardButton.checked) {
            var tpos = cpos - (1/fps)
            if(tpos < stime) {
                timer.stop()
                cpos = stime     
            } else {
                cpos = tpos
            }
        }

    }

    function on_pause(){
        //pauseButton.checked=true
        pause()
    }

    function on_rewind(){
        cpos = stime
    }

    function on_fastforward(){
        cpos = etime
    }

    function on_back_frame() {
        var tpos = cpos - (1/fps)
        if(tpos < stime)
            cpos = stime
        else
            cpos = tpos
    }

    function on_forward_frame() {
        var tpos = cpos + (1/fps)
        if(tpos > etime)
            cpos = etime
        else
            cpos = tpos
    }

    Component.onCompleted: {
        playForwardButton.buttonPressed.connect(on_play_forward)
        playBackwardButton.buttonPressed.connect(on_play_backward)
        rewindButton.buttonPressed.connect(on_rewind)
        fastForwardButton.buttonPressed.connect(on_fastforward)
        backFrameButton.buttonPressed.connect(on_back_frame)
        forwardFrameButton.buttonPressed.connect(on_forward_frame)

        addKeyButton.buttonPressed.connect(on_add_key)

        timer.triggered.connect(update_play)
        //pauseButton.buttonPressed.connect(on_pause)
    }
}
