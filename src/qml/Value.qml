import QtQuick 2.7


Item {
    id: box 
    width: 100
    height: 17

    property int uid: 0
    property int nid: 0
    property int fid: 0 
    property alias value: value.text
    property bool showTip: false
    property alias tipMsg: tip.msg
 
    signal valuePressed()
    signal valueReleased()


    Theme { id: theme }


    Tooltip { id: tip }


    Rectangle {
        id: background 
        anchors.fill: parent
        color: theme.valueBackgroundNoKeyColor 
        border.color: theme.valueBorderNormalColor
        border.width: theme.valueBorderNormalWidth
        radius: theme.valueBorderNormalRadius

        Text {
            id: value 
            anchors.fill: parent.fill
            anchors.centerIn: parent
            text: "0.0"
            color: theme.valueTextNormalColor
        }


    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
       
        onEntered: {
            background.color = theme.valueBackgroundHoverColor
            value.color = theme.valueTextHoverColor
            var gp = mapToGlobal(background.x,0)
            tip.x = gp.x + width
            tip.y = gp.y
            tip.visible = true
        }

        onPressed: {
            background.color = theme.valueBackgroundPressedColor
            label.color = theme.valueTextPressedColor
            valuePressed()
        }

        onReleased: {
            background.color = theme.valueBackgroundHoverColor
            value.color = theme.valueTextHoverColor
            valueReleased()
        }

        onExited: {
            background.color = theme.valueBackgroundNoKeyColor
            value.color = theme.valueTextNormalColor
            tip.visible = false
        }

    }

}
