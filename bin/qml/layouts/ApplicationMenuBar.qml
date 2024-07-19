import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.theme as ThemeModule

MenuBar {
    required property ThemeModule.CircularChanger themeChanger

    id: menubar
    Material.background: App.Theme.palette.base1
    padding: 0
    topInset: 0
    bottomInset: 0
    Menu {
        Material.background: App.Theme.palette.base1
        padding: 0
        topInset: 0
        bottomInset: 0
        title: "Файл"
        contentWidth: 300

        Action {
            text: "Закрыть программу"
            onTriggered: Qt.quit()
        }
    }

    ToolButton {
        parent: menubar
        anchors.right: parent.right
        icon {
            source: App.Theme.mode === App.Theme.Dark ? "qrc:/resources/icons/light.svg" : "qrc:/resources/icons/dark.svg"
        }
        flat: true
        z: parent.z + 1
        onPressed: menubar.themeChanger.toggle(this)
    }
}