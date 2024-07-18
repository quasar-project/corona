import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.theme as ThemeModule
import io.corona.standalone.imgui as ImguiModule
import io.corona.standalone.map as MapModule

// import "layouts" as Layouts
// import "utils" as Utils
// import "widgets" as Widgets // todo: move to module

ApplicationWindow {
    id: window_root

    title: "Corona"
    minimumWidth: 720
    minimumHeight: 360
    width: 1500
    height: 800
    visible: true
    color: App.Theme.palette.base0

    Component.onCompleted: {
        console.info("application ui loaded")
        showMaximized()
    }

    Material.theme: App.Theme.mode === App.Theme.Dark ? Material.Dark : Material.Light
    Material.background: App.Theme.palette.base2
    Material.foreground: App.Theme.palette.text
    Material.accent: App.Theme.palette.mauve
    Material.primary: App.Theme.palette.sky
    Material.roundedScale: Material.ExtraSmallScale

    Rectangle {
        id: root
        color: App.Theme.palette.base0
        anchors.fill: parent

        MapModule.MapView {
            id: mapView
            anchors.fill: parent
            imguiRenderer: imguiRenderer
        }

        Button {
            id: button
            height: 100
            text: App.Theme.mode === App.Theme.Dark ? "Dark" : "Light"
            anchors.centerIn: parent
            onClicked: themeChanger.toggle(this)
        }
    }

    ImguiModule.ImmediateGuiMainRenderer { id: imguiRenderer; shortcut: "F5"; opacity: 0 }
    ThemeModule.CircularChanger { id: themeChanger; rootItem: root; theme: App.Theme; }
}

