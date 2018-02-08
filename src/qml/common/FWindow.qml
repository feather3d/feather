import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml.Models 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: window
    width: 300
    height: 100
 
    property alias title: titletext.text
    //property alias showMenu: mainMenu.visible
    //property alias showToolBar: toolbar.visible
    //property alias tinyToolBar: toolbar.tiny


    FTheme { id: theme }


    ObjectModel { id: containerModel }


    Rectangle {
        id: frame 
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom 
        color: theme.windowBackgroundNormalColor 
        border.color: theme.windowBorderNormalColor
        border.width: 1
        radius: 5

        Text {
            id: titletext
            text: 'window'
            height: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 12
            color: theme.windowTitleBarTextNormalColor
        }


        Column { 
            id: row
            anchors.fill: parent
            anchors.topMargin: 20
            anchors.bottomMargin: 20

            /*

            Component {
                id: menuBarStyle

                MenuBarStyle {

                    background: Rectangle {
                        width: 300
                        height: 20
                        color: "grey"
                        border.color: "black"
                        border.width: 1
                        radius: 2
                    }
                }

            }
            */

            /*
            MenuBar {
                id: mainMenu
                style: menuBarStyle 

                Menu {
                    id: fileMenu
                    title: "File"
                    visible: true
                }
                
            } 
            */

            /*
            MenuBar {
                id: menubar
                width: row.width
                height: (visible) ? 20 : 0
            }
            */

            /*
            ToolBar {
                id: toolbar
                width: row.width
                height: (visible) ? ((tiny) ? 36 : 72) : 0
            }
            */
              
            Rectangle {
                id: container
                width: row.width
                height: row.height
                color: theme.windowContainerBackgroundNormalColor 
                border.width: 1
                border.color: '#606060'

                ListView {
                    id: centralWidget
                    anchors.fill: parent
                    model: containerModel
                }
            }
        }


        Rectangle {
            id: resizer
            width: 10
            height: 10
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 5
            radius: 5
            color: 'lightgrey'
 
            MouseArea {
                id: resizer_mousearea
                anchors.fill: parent
                property bool resizeActive: false
                acceptedButtons: Qt.LeftButton | Qt.RightButton
 
                onPositionChanged: {
                    window.width = window.width + mouse.x
                    window.height = window.height + mouse.y
                    resizer.color = 'lime'
                }

                onReleased: {
                    resizer.color = 'lightgrey'
                }
            }
        }

        MouseArea {
            id: move_mousearea
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 20
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            property int xoffset: 0
            property int yoffset: 0
            hoverEnabled: true
            property bool moveEnabled: false

            onPressed: {
                xoffset = mouse.x
                yoffset = mouse.y
                moveEnabled = true
                frame.color = theme.windowTitleBarBackgroundActiveColor
                titletext.color = theme.windowTitleBarTextActiveColor
            }

            onPositionChanged: {
                if(moveEnabled) {
                    window.x = window.x + mouse.x - xoffset
                    window.y = window.y + mouse.y - yoffset
                }
            }

            onReleased: {
                frame.color = theme.windowTitleBarBackgroundNormalColor
                titletext.color = theme.windowTitleBarTextNormalColor 
                xoffset = 0
                yoffset = 0
                moveEnabled = false
            }
        }
    }


    // window functions


    function addWidget(widget) {
        centralWidget.model.append(widget)
    }

    function centralWidget() {
        return centralWidget
    }

    /*
    function menubar() {
        return menubar
    }
    */

    /*
    function toolbar() {
        return toolbar
    }
    */

}
