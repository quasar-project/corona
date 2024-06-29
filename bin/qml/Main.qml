import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtLocation
import QtPositioning
import QtQuick.Layouts

import io.corona.standalone.theme as MTheme

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
    color: MTheme.Theme.palette.base0

    Component.onCompleted: {
        //showMaximized()
    }

    Material.theme: MTheme.Theme.mode === MTheme.Theme.Dark ? Material.Dark : Material.Light
    Material.background: MTheme.Theme.palette.base2
    Material.foreground: MTheme.Theme.palette.text
    Material.accent: MTheme.Theme.palette.mauve
    Material.primary: MTheme.Theme.palette.sky
    Material.roundedScale: Material.ExtraSmallScale

    Rectangle {
        id: root
        color: MTheme.Theme.palette.base0
        anchors.fill: parent

        Button {
            id: button
            height: 100
            text: MTheme.Theme.mode === MTheme.Theme.Dark ? "Dark" : "Light"
            anchors.centerIn: parent
            onClicked: themeChanger.toggle(this)
        }

        Button {
            id: button2
            height: 100
            text: MTheme.Theme.mode === MTheme.Theme.Dark ? "Dark" : "Light"
            anchors.right: parent.right
            onClicked: themeChanger.toggle(this)
        }
    }

    MTheme.CircularChanger { id: themeChanger; rootItem: root; theme: MTheme.Theme; }
}

