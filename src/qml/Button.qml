import QtQuick 2.7


Item {
    id: button
    width: 100
    height: 17
 
    property alias text: label.text
    property bool showTip: false
    property alias tipMsg: tip.msg
 
    signal buttonPressed()
    signal buttonReleased()

    Rectangle {
        id: background 
        anchors.fill: parent
        color: theme.buttonBackgroundNormalColor 
        border.color: theme.buttonBorderNormalColor
        border.width: theme.buttonBorderNormalWidth
        radius: theme.buttonBorderNormalRadius
    }

    Theme { id: theme }

    Text {
        id: label
        anchors.fill: parent.fill
        anchors.centerIn: parent
        text: ""
        color: theme.buttonTextNormalColor
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
       
        onEntered: {
            background.color = theme.buttonBackgroundHoverColor
            label.color = theme.buttonTextHoverColor
            var gp = mapToGlobal(background.x,0)
            tip.x = gp.x + width
            tip.y = gp.y
            tip.visible = true
        }

        onPressed: {
            background.color = theme.buttonBackgroundPressedColor
            label.color = theme.buttonTextPressedColor
            buttonPressed()
        }

        onReleased: {
            background.color = theme.buttonBackgroundHoverColor
            label.color = theme.buttonTextHoverColor
            buttonReleased()
        }

        onExited: {
            background.color = theme.buttonBackgroundNormalColor
            label.color = theme.buttonTextNormalColor
            tip.visible = false
        }

    }

    Tooltip { id: tip }
}
