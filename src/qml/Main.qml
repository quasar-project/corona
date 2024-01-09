import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtLocation
import QtPositioning
import QtQuick.Layouts

import Corona.Config
import Corona.Gui.Theme

import "layouts" as Layouts

ApplicationWindow {
    id: window_root

    title: "Corona"
    minimumWidth: 720
    minimumHeight: 360
    width: 1500
    height: 800
    visible: true
    color: Theme.io.color("crust")

    Component.onCompleted: {
        showMaximized()
    }

    Material.theme: Theme.mode === Theme.Dark ? Material.Dark : Material.Light
    Material.background: Theme.io.color("background")
    Material.foreground: Theme.io.color("foreground")
    Material.accent: Theme.io.color("accent")
    Material.primary: Theme.io.color("main")

    // menuBar: Layouts.MenuApplicationBar {}

    ToolButton {
        //anchors.right: parent.right
        //parent: window_root.menuBar
        anchors.centerIn: parent
        icon {
            source: Theme.mode === Theme.Dark ? "qrc:/icons/common/light.svg" : "qrc:/icons/common/dark.svg"
        }
        flat: true
        onPressed: themeChanger.toggleTheme(this)
    }

    // Item {
    //     id: root
    //     anchors.fill: parent
    //
    //     Layouts.MapView {
    //         id: map
    //
    //         anchors {
    //             top: parent.top
    //             left: parent.left
    //             right: parent.right
    //             bottom: parent.bottom
    //         }
    //     }
    // }

    // QtxThemeChanger { id: themeChanger; z: 65535; rootItem: root }
}

