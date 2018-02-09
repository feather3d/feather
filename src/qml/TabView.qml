import QtQuick 2.7


Item {
    id: tabview 
    width: 400
    height: 400


    StackView {
        id: view


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
