import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml.Models 2.3

Window {
    id: window
    width: tip.contentWidth + 4 
    height: tip.contentHeight + 4 
    flags: Qt.Popup
    visible: false
    color: 'transparent'

    property alias msg: tip.text


    Rectangle {
        id: background 
        //width: ((tip.paintedWidth+4) > 100) ? 100 : tip.paintedWidth
        //height: ((tip.paintedWidth+4) > 100) ? 15 * (tip.paintedWidth+4)/100 : 15 
        //width: tip.contentWidth + 4 
        //height: tip.contentHeight + 4 
        anchors.fill: parent
        color: 'orange'
        border.color: 'grey'
        border.width: 1 
        radius: 2
        opacity: 0.75


        Text {
            id: tip
            anchors.fill: parent.fill
            wrapMode: Text.WordWrap
            padding: 2
            font.pixelSize: 9
            color: 'black'
        }
        
    }

}
