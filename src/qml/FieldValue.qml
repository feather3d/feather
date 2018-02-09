import QtQuick 2.7


Item {
    id: field 
    width: 100
    height: 17

    property int valuetype: 1 
    property alias text: label.text
    property bool showTip: false
    property alias tipMsg: tip.msg
 
    signal labelPressed()
    signal labelReleased()
    signal valuePressed()
    signal valueReleased()


    Theme { id: theme }


    Tooltip { id: tip }


    Rectangle {
        id: background 
        anchors.fill: parent
        color: get_type_color() 
        //border.color: theme.buttonBorderNormalColor
        //border.width: theme.buttonBorderNormalWidth
        radius: 2 

        Row {
            id: valueRow
            anchors.left: parent.left
            anchors.leftMargin: 4
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Text {
                id: label
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: valueRow.width - value.width 
                text: "----"
                horizontalAlignment: Text.AlignHLeft
                verticalAlignment: Text.AlignVCenter
                
                color: theme.buttonTextNormalColor
            }

            Value {
                id: value
                width: 50
                anchors.top: parent.top
                anchors.topMargin: 1
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
            }
        }

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
            valuePressed()
        }

        onReleased: {
            background.color = theme.buttonBackgroundHoverColor
            label.color = theme.buttonTextHoverColor
            valueReleased()
        }

        onExited: {
            background.color = get_type_color() 
            label.color = theme.buttonTextNormalColor
            tip.visible = false
        }

    }

    
    function get_type_color() {
        switch(valuetype) {
            case 0:
                return "#00ffff"
            case 1:
                return "#ff00ff"
        }
        return "#ffffff"
    }
}
