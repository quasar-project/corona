import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtLocation
import QtPositioning
import QtQuick.Layouts

import QtExtensions 1.0
import QtExtensions.Toolkit 1.0
import Quasar 1.0

import "layouts" as Layouts
//import "windows" as Windows

ApplicationWindow {
    property string mainfont: __fontmain.name
    property string monofont: __fontmono.name

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
    }

    Material.theme: Theme.darkMode === Theme.Dark ? Material.Dark : Material.Light
    Material.background: Theme.io.color(Theme.Base)
    Material.foreground: Theme.io.color(Theme.Text)
    Material.accent: Theme.io.color(Theme.Mauve)

    XOsmProvider { id: provider }

    FontLoader { id: __fontmain; source: "qrc:/fonts/Overpass.ttf" }
    FontLoader { id: __fontmono; source: "qrc:/fonts/Inconsolata.ttf" }

    menuBar: Layouts.MenuApplicationBar {}

    ToolButton {
        anchors.right: parent.right
        parent: window_root.menuBar
        icon {
            source: Theme.darkMode == Theme.Dark ? "qrc:/icons/common/light.svg"
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

    Layouts.DebugNetworkPanel {
        id: debugNetworkPanel
        anchors {
            top: parent.top
            left: parent.left
        }
    }

    QtxThemeChanger { id: themeChanger; z: 65535; rootItem: root }
}

