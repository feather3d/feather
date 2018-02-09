import QtQuick 2.7


Item {
    id: button
    width: image.paintedWidth 
    height: image.paintedHeight

    property alias path: image.source
    property alias frameVisible: background.visible
    property bool showTip: false
    property alias tipMsg: tip.msg
    // TODO
    property bool showBorder: true
  
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


    Image {
        id: image 
        //anchors.fill: parent.fill
        //anchors.centerIn: parent
        source: ""
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
       
        onEntered: {
            background.color = theme.buttonBackgroundHoverColor
            image.scale = 1.1
            //label.color = theme.buttonTextHoverColor
            var gp = mapToGlobal(background.x,0)
            tip.x = gp.x + width
            tip.y = gp.y
            tip.visible = true
        }

        onPressed: {
            background.color = theme.buttonBackgroundPressedColor
            //label.color = theme.buttonTextPressedColor
            buttonPressed()
        }

        onReleased: {
            background.color = theme.buttonBackgroundHoverColor
            //label.color = theme.buttonTextHoverColor
            buttonReleased()
        }

        onExited: {
            background.color = theme.buttonBackgroundNormalColor
            //label.color = theme.buttonTextNormalColor
            image.scale = 1
            tip.visible = false 
        }

    }


    Tooltip { id: tip }


    Component.onCompleted: {
    }
}
