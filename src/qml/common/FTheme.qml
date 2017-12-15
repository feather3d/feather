import QtQuick 2.7

/*
 * This contains all the colors used by all the widgets in the application.
 * All the colors are named in this way:
 *      [widget][subsection][part][state][property]
 *
 * Widgets:
 *      window
 *      button
 *      menu
 *
 * Subsection: \\ this is optional and is used to tell different parts of the widget
 *
 * Parts:
 *      border
 *      background
 *      foreground
 *      text
 *
 * States:
 *      normal
 *      hover
 *      pressed
 *      active
 *      disabled
 *
 * Properties:
 *      color
 *      size 
 *
 */


Item {
    id: themeModel 

    // window 

    // border
    property string windowBorderNormalColor: "#000000"
    property int windowBorderNormalWidth: 1 
    property int windowBorderNormalRadius: 4 
    // background
    property string windowBackgroundNormalColor: "#707070"
    // title bar 
    property string windowTitleBarBackgroundNormalColor: "#707070"
    property string windowTitleBarBackgroundActiveColor: "#a0a0a0"
    property string windowTitleBarBackgroundDisabledColor: "#000000"
    // title bar text
    property string windowTitleBarTextNormalColor: "#000000"
    property string windowTitleBarTextActiveColor: "#000000"
    property string windowTitleBarTextDisabledColor: "#000000"
    // message bar 
    property string windowMessageBarBackgroundNormalColor: "#707070"
    property string windowMessageBarBackgroundActiveColor: "#000000"
    property string windowMessageBarBackgroundDisabledColor: "#000000"
    // message bar text
    property string windowMessageBarTextNormalColor: "#000000"
    property string windowMessageBarTextActiveColor: "#000000"
    property string windowMessageBarTextDisabledColor: "#000000"
    // container
    property string windowContainerBackgroundNormalColor: "#808080"
    property string windowContainerBackgroundActiveColor: "#000000"
    property string windowContainerBackgroundDisabledColor: "#000000"


    // menu 
    // border
    property string menuBorderColor: "#000000"
    property int menuBorderWidth: 1 
    property int menuBorderRadius: 2 
    // background
    property string menuBackgroundColor: "#707070"
    // text
    property string menuTextNormalColor: "#000000"
    property string menuTextHoverColor: "#000000"
    property string menuTextActiveColor: "#000000"
    property string menuTextPressedColor: "#000000"
    property string menuTextDisabledColor: "#000000"


    // button
    // border
    property string buttonBorderNormalColor: "#000000"
    property int buttonBorderNormalWidth: 1 
    property int buttonBorderNormalRadius: 4 
    // background
    property string buttonBackgroundNormalColor: "#707070"
    property string buttonBackgroundHoverColor: "#a0a0a0"
    property string buttonBackgroundActiveColor: "#c0c0c0"
    property string buttonBackgroundPressedColor: "#f0f0f0"
    property string buttonBackgroundDisabledColor: "#404040"
    // text
    property string buttonTextNormalColor: "#000000"
    property string buttonTextHoverColor: "#000000"
    property string buttonTextActiveColor: "#000000"
    property string buttonTextPressedColor: "#000000"
    property string buttonTextDisabledColor: "#000000"


    // splitView
    // border
    property string splitViewHandleBorderColor: "#000000"
    property int splitViewHandleBorderWidth: 1 
    property int splitViewHandleBorderRadius: 2 
    property int splitViewHandleWidth: 6 
    // background
    property string splitViewHandleBackgroundNormalColor: "#707070"
    property string splitViewHandleBackgroundHoverColor: "#a0a0a0"
    property string splitViewHandleBackgroundActiveColor: "#c0c0c0"
    property string splitViewHandleBackgroundPressedColor: "#f0f0f0"


    // value
    // border
    property string valueBorderNormalColor: "#000000"
    property int valueBorderNormalWidth: 1 
    property int valueBorderNormalRadius: 4 
    // background
    property string valueBackgroundNoKeyColor: "#e1e1e1"
    property string valueBackgroundLockedColor: "#707070"
    property string valueBackgroundOnTrackColor: "#ffbf64"
    property string valueBackgroundOnKeyColor: "#ff00ff"
    property string valueBackgroundHoverColor: "#a0a0a0"
    property string valueBackgroundActiveColor: "#c0c0c0"
    property string valueBackgroundPressedColor: "#f0f0f0"
    property string valueBackgroundDisabledColor: "#404040"
    // text
    property string valueTextNormalColor: "#000000"
    property string valueTextHoverColor: "#000000"
    property string valueTextActiveColor: "#000000"
    property string valueTextPressedColor: "#000000"
    property string valueTextDisabledColor: "#000000"

    // groupbox
    // border
    property string groupBoxBorderNormalColor: "#000000"
    property int groupBoxBorderNormalWidth: 1 
    property int groupBoxBorderNormalRadius: 4 
    // background
    property string groupBoxBackgroundNormalColor: "#808080"
    property string groupBoxBackgroundHoverColor: "#a0a0a0"
    property string groupBoxBackgroundActiveColor: "#c0c0c0"
    property string groupBoxBackgroundPressedColor: "#f0f0f0"
    property string groupBoxBackgroundDisabledColor: "#404040"
    // label background
    property string groupBoxLabelBackgroundNormalColor: "#909090"
    property string groupBoxLabelBackgroundHoverColor: "#a0a0a0"
    property string groupBoxLabelBackgroundActiveColor: "#c0c0c0"
    property string groupBoxLabelBackgroundPressedColor: "#f0f0f0"
    property string groupBoxLabelBackgroundDisabledColor: "#404040"
    // label text 
    property string groupBoxLabelTextNormalColor: "#000000"
    property string groupBoxLabelTextHoverColor: "#000000"
    property string groupBoxLabelTextActiveColor: "#000000"
    property string groupBoxLabelTextPressedColor: "#000000"
    property string groupBoxLabelTextDisabledColor: "#000000"



    function load_properties() {
        // TODO - add the ability to load theme files 
    }
}
