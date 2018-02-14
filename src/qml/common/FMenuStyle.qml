import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml.Models 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import feather.command 1.0
import feather.scenegraph 1.0
import feather.node 1.0
import feather.tools 1.0

MenuStyle {

    FTheme { id: theme }

    property Properties properties: Null

    frame: Rectangle {
        color: theme.menuBackgroundColor
        border.color: theme.menuBorderColor
        border.width: theme.menuBorderWidth
        radius: theme.menuBorderRadius
    }
    
    font.pixelSize: theme.menuFontPixelSize

    itemDelegate.label: Text {
        text: styleData.text 
        font.pixelSize: theme.menuFontPixelSize 
        color: theme.menuFontColor
    }
}

