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
import "utils" as Utils
import "widgets" as Widgets // todo: move to module

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
    Material.roundedScale: Material.ExtraSmallScale

    menuBar: Layouts.MenuApplicationBar {}

    ToolButton {
        anchors.right: parent.right
        parent: window_root.menuBar
        icon {
            source: Theme.mode === Theme.Dark ? "qrc:/icons/common/light.svg" : "qrc:/icons/common/dark.svg"
        }
        flat: true
        onPressed: themeChanger.toggle(this)
    }

    Rectangle {
        id: root
        anchors.fill: parent
        color: Theme.io.color("crust")

        Layouts.MapView {
            id: map

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }

    Widgets.PowerSwitchStatus {
        anchors.centerIn: parent
    }

    Utils.ThemeCircularPaletteReveal { id: themeChanger; z: 65535; rootItem: root }
}

