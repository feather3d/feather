import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml.Models 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Component {
    id: menuBarStyle

    MenuBarStyle {

        background: Rectangle {
            color: theme.menuBackgroundColor 
            border.color: theme.menuBorderColor
            border.width: theme.menuBorderWidth
            radius: theme.menuBorderRadius
        }
    }

}


