import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtLocation
import QtPositioning
import QtQuick.Layouts

import Corona.Config

import "layouts" as Layouts

ApplicationWindow {
    id: window_root

    title: "Corona"
    minimumWidth: 720
    minimumHeight: 360
    width: 1500
    height: 800
    visible: true
    font.family: mainfont

    Component.onCompleted: {
        showMaximized()
        console.info(Config.value("ip", "de10"))
    }

    Material.theme: Theme.darkMode === Theme.Dark ? Material.Dark : Material.Light
    Material.background: Theme.io.color(Theme.Base)
    Material.foreground: Theme.io.color(Theme.Text)
    Material.accent: Theme.io.color(Theme.Mauve)

    // menuBar: Layouts.MenuApplicationBar {}

    ToolButton {
        anchors.right: parent.right
        parent: window_root.menuBar
        icon {
            source: Theme.darkMode === Theme.Dark ? "qrc:/icons/common/light.svg"
                                                  : "qrc:/icons/common/dark.svg"
        }
        flat: true
        onPressed: themeChanger.toggleTheme(this)
    }

    Item {
        id: root
        anchors.fill: parent

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

    // QtxThemeChanger { id: themeChanger; z: 65535; rootItem: root }
}

