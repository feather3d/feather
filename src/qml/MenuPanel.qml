import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml.Models 2.3


Item {
    id: panel
    width: 0
    height: 0
    visible: false

    property string name: "default" 


    ListModel { id: menuModel }


    ObjectModel { id: menuPanel }


    Component {
        id: menuDelegate

        Rectangle {
            id: menuItem
            width: menuText.contentWidth 
            height: menuText.contentHeight 
            color: 'transparent'

            Text {
                id: menuText;
                text: name
            }

            
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: { menuItem.color = "grey"; showPanel(index,menuItem.x,menuItem.y) }
                onExited: { menuItem.color = "transparent"; showPanel(menuView.model.count,0,0) }
            }
        }
    }


    Window {
        id: window
        width: 100
        height: menuModel.count * 17
        flags: Qt.Popup
        visible: false
        color: 'transparent'


        Rectangle {
            id: frame
            anchors.fill: parent
            color: '#a0a0a0'
            border.color: 'black'
            border.width: 1 
            onVisibleChanged: { console.log("visible set to " + visible + " and XY=" + x + "x" + y + " and width:" +  width + "x" + height + " for panel " + name) }


            ListView {
                id: menuView
                anchors.fill: parent
                anchors.topMargin: 2
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.bottomMargin: 2
                model: menuModel
                delegate: menuDelegate
                orientation: ListView.Vertical
                spacing: 8
            }


            ListView {
                id: panelView
                anchors.fill: parent.fill
                model: menuPanel
            }


            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onExited: { panel.visible = false } 
            }

        }

    }

    onVisibleChanged: { window.visible = visible }

    onXChanged: { window.x = x }

    onYChanged: { window.y = y }


    function addMenuItem(item) {
        menuView.model.append(item)
        panelView.model.append(item)
    }

}
