import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import QtExtensions.Toolkit 1.0
import Quasar 1.0

MenuBar {
    Material.background: Theme.io.color(Theme.Mantle)
    padding: 0
    topInset: 0
    bottomInset: 0
    Menu {
        Material.background: Theme.io.color(Theme.Mantle)
        padding: 0
        topInset: 0
        bottomInset: 0
        title: "Файл"
        contentWidth: 300

        Action {
            text: "Закрыть программу"
            icon {
                //source: "qrc:/icons/vector/common/close.svg"
                color: Theme.io.color(Theme.Text)
            }
            onTriggered: Qt.quit()
        }
    }
}