import QtQuick 2.7


Item {
    id: tab 
    width: 200
    height: 200
 
    property alias label: label.text
    property bool showTip: false
    property alias tipMsg: tip.msg
    property int index: 0
 
    signal panelPressed(int index)
    signal panelReleased(int index)


    Theme { id: theme }


    Tooltip { id: tip }


    Rectangle {
        id: labelBackground
        width: 100
        height: 22 
        x:0
        anchors.top: parent.top 
        color: theme.buttonBackgroundNormalColor 
        border.color: theme.buttonBorderNormalColor
        border.width: theme.buttonBorderNormalWidth
        radius: theme.buttonBorderNormalRadius


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
                panelBackground.color = theme.buttonBackgroundHoverColor
                label.color = theme.buttonTextHoverColor
                var gp = mapToGlobal(panelBackground.x,0)
                tip.x = gp.x + width
                tip.y = gp.y
                tip.visible = true
            }

            onPressed: {
                panelBackground.color = theme.buttonBackgroundPressedColor
                label.color = theme.buttonTextPressedColor
                panelPressed(index)
            }

            onReleased: {
                panelBackground.color = theme.buttonBackgroundHoverColor
                label.color = theme.buttonTextHoverColor
                panelReleased(index)
            }

            onExited: {
                panelBackground.color = theme.buttonBackgroundNormalColor
                label.color = theme.buttonTextNormalColor
                tip.visible = false
            }

        }

    }



    Rectangle {
        id: panelBackground 
        anchors.top: parent.top
        anchors.topMargin: 17 
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: theme.buttonBackgroundNormalColor 
        border.color: theme.buttonBorderNormalColor
        border.width: theme.buttonBorderNormalWidth
        radius: theme.buttonBorderNormalRadius


        // this strip hides the line between the label and panel
        Rectangle {
            id: hidestrip
            anchors.top: parent.top
            x: 0
            width: labelBackground.width
            height: theme.buttonBorderNormalWidth + 2
            //anchors.margins: theme.buttonBorderNormalWidth
            color: theme.buttonBackgroundNormalColor 
            radius: theme.buttonBorderNormalRadius
        }


        Rectangle {
            id: containerBackground 
            anchors.fill: parent
            //visible: false
            anchors.margins: theme.buttonBorderNormalWidth
            color: theme.buttonBackgroundNormalColor 
            radius: theme.buttonBorderNormalRadius
        }

    }

}
