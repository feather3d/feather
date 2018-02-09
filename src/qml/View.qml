import QtQuick 2.7

WindowPanel {
    id: view
    title: "view_window"
    width: 400
    height: 600
    //showMenu: false
    //showToolBar: false
    //tinyToolBar: true 


    Item {
        id: layout
        width: centralWidget().width
        height: centralWidget().height

                
        FieldValue { id: tx; text: "tx"; tipMsg: "translat x" }
        FieldValue { id: ty; text: "ty" }
        FieldValue { id: tz; text: "tz" }

        FieldValue { id: rx; text: "rx"; tipMsg: "rotate x" }
        FieldValue { id: ry; text: "ry" }
        FieldValue { id: rz; text: "rz" }


        Column {
            id: column 
            spacing: 2
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 4

            Button { id: button1; text: "Button 1"; showTip: true; tipMsg: "button one" }


            Button { id: button2; text: "Button 2"; showTip: true; tipMsg: "button two this is a long message that is <b>supposed</b> \n to go on for a good bit." }


            ImageButton { id: button3; path: "icons/axis.svg"; frameVisible: false; showTip: true; tipMsg: "image button" }


            GroupBox {
                id: tranGroupBox
                label: "translation"
            }


            GroupBox {
                id: rotGroupBox
                label: "rotation"
            }


            TabPanel { id: tab1; label: "tab1" }


        }

    }

    ToolItem{ id: tool1; icon: "icons/bezier_curve.svg"; label: "tool1"; tipMsg: "tool1 tip" }

    ToolItem{ id: tool2; icon: "icons/add_key.svg"; label: "tool2"; tipMsg: "tool2 tip" }


    // Menus

    // File Menu
    MenuPanel{ id: fileMenu; name: 'File' }
    MenuItem { id: openMenuItem; name: "Open" }
    MenuItem { id: exportMenuItem; name: "Export" }

    // Edit Menu
    MenuPanel{ id: editMenu; name: 'Edit' }
    MenuItem { id: selectAllMenuItem; name: "Select All" }
    MenuItem { id: invertSelectionMenuItem; name: "Invert Selection" }


    // Window Menu
    MenuPanel{ id: windowMenu; name: 'Window' }
    MenuItem { id: editorMenuItem; name: "Editor" }
    MenuItem { id: outlinerMenuItem; name: "Outliner" }


    Component.onCompleted: {
        addWidget(layout)
        toolbar().addItem(tool1)
        toolbar().addItem(tool2)

        fileMenu.addMenuItem(openMenuItem)
        fileMenu.addMenuItem(exportMenuItem)
        editMenu.addMenuItem(selectAllMenuItem)
        editMenu.addMenuItem(invertSelectionMenuItem)
        windowMenu.addMenuItem(editorMenuItem)
        windowMenu.addMenuItem(outlinerMenuItem)

        menubar().addMenuItem(fileMenu)
        menubar().addMenuItem(editMenu)
        menubar().addMenuItem(windowMenu)

        tranGroupBox.addWidget(tx)
        tranGroupBox.addWidget(ty)
        tranGroupBox.addWidget(tz)

        rotGroupBox.addWidget(rx)
        rotGroupBox.addWidget(ry)
        rotGroupBox.addWidget(rz)


    }
}
