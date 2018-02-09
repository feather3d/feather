import QtQuick 2.7

Rectangle {
    id: frame
    width: 100
    height: 17
    color: '#a0a0a0'
    border.color: 'black'
    border.width: 1 

    property alias name: label.text


    Theme { id: theme }


    MenuPanel { id: menu }


    Text {
        id: label
        anchors.fill: parent.fill
        anchors.centerIn: parent
        text: ""
        color: theme.buttonTextNormalColor
    }

    function addMenuItem(item) {
        menu.addMenuItem(item)
    }

}
