import QtQuick 2.7


Item {
    id: button
    width: (tiny) ? 33 : 66
    height: (tiny) ? 33 : 66 

    property alias icon: image.source
    property alias label: label.text 
    property alias frameVisible: background.visible
    property bool showTip: false
    property alias tipMsg: tip.msg 
    property bool tiny: false
    
    // TODO
    property bool showBorder: true
  
    signal buttonPressed()
    signal buttonReleased()

    Theme { id: theme }

    Tooltip { id: tip }

    Rectangle {
        id: background 
        anchors.fill: parent
        color: theme.buttonBackgroundNormalColor 
        border.color: theme.buttonBorderNormalColor
        border.width: theme.buttonBorderNormalWidth
        radius: theme.buttonBorderNormalRadius


        Image {
            id: image 
            width: (tiny) ? 32 : 64
            height: (tiny) ? 32 : 64
            anchors.fill: parent.fill
            anchors.centerIn: parent
            source: ""
        }


        Rectangle {
            height: 15
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 1
            color: theme.buttonBackgroundNormalColor 
            opacity: 0.75
            visible: (tiny) ? false : true
     
            Text {
                id: label
                anchors.fill: parent.fill
                anchors.centerIn: parent
            }
        }
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
       
        onEntered: {
            background.color = theme.buttonBackgroundHoverColor
            background.scale = 1.1
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
            background.scale = 1
            tip.visible = false
        }

    }

}
