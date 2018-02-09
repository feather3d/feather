import QtQuick 2.7
import QtQml.Models 2.3


Item {
    id: groupbox 
    width: 200
    height: (!expanded) ? labelbox.height : (box.height + labelbox.height/2)
 
    property alias label: label.text
    property bool showTip: false
    property alias tipMsg: tip.msg
    property bool expanded: true
 
    signal buttonPressed()
    signal buttonReleased()


    Theme { id: theme }


    Tooltip { id: tip }


    ObjectModel { id: containerModel }


    Rectangle {
        id: box 
        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 2
        height: (groupWidget.visible) ? ((!containerModel.count) ? 5 : ((containerModel.get(containerModel.count-1).y - containerModel.get(0).y) + containerModel.get(containerModel.count-1).height + 12 + 4)) : 5
        color: theme.groupBoxBackgroundNormalColor 
        border.color: theme.groupBoxBorderNormalColor
        border.width: theme.groupBoxBorderNormalWidth
        radius: theme.groupBoxBorderNormalRadius

        ListView {
            id: groupWidget 
            anchors.fill: parent
            anchors.topMargin: 12
            model: containerModel
            spacing: 2
        }

    }


    Rectangle {
        id: labelbox
        width: 100
        height: 20
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.left: parent.left
        anchors.leftMargin: 20
        color: theme.groupBoxLabelBackgroundNormalColor 
        border.color: theme.groupBoxBorderNormalColor
        border.width: theme.groupBoxBorderNormalWidth
        radius: theme.groupBoxBorderNormalRadius
 
        Text {
            id: label
            anchors.fill: parent.fill
            anchors.centerIn: parent
            color: theme.groupBoxLabelTextNormalColor
        }
        
        Rectangle {
            id: expander 
            width: 10
            height: 10
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 5
            radius: 5
            color: 'lightgrey'
 
            MouseArea {
                id: expander_mousearea
                anchors.fill: parent
                property bool expandActive: false
                acceptedButtons: Qt.LeftButton | Qt.RightButton
 
                onPressed: {
                    if(groupWidget.visible) {
                        groupWidget.visible = false
                        expanded = false
                    } else {
                        groupWidget.visible = true
                        expanded = true
                    }
                    expander.color = 'lime'
                }

                onReleased: {
                    expander.color = 'lightgrey'
                }
            }
        }

    }

   
    function addWidget(widget) {
        containerModel.append(widget)
        widget.x = 2
        widget.width = groupWidget.width - 4
    }
}
