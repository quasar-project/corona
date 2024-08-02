import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.ui as UI
import io.corona.standalone.map as MapModule

Item {
    UI.SimpleCheckableToolButton {
        id: layersButton
        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 12
        }
        iconPath: "qrc:/resources/icons/layers.svg"
        changeOnCheck: false
    }

    Pane {
        property bool shown: layersButton.checked

        id: layersPane
        anchors {
            bottom: layersButton.bottom
            top: layersButton.top
            right: layersButton.left
            rightMargin: 12
        }
        width: this.shown ? implicitWidth : 0
        visible: width > 0
        Behavior on width { NumberAnimation { easing.type: Easing.InOutQuad; duration: 100 } }
        clip: true
        topInset: 0
        bottomInset: 0
        Material.elevation: 30
        Material.background: App.Theme.palette.base2
        Material.foreground: App.Theme.palette.text
        Material.roundedScale: Material.ExtraSmallScale

        RowLayout {
            anchors.fill: parent

            Button {
                text: "Схема"
                Material.background: App.Theme.palette.base2
                Material.foreground: App.Theme.palette.text
                Material.roundedScale: Material.ExtraSmallScale
                onPressed: MapModule.MapManager.mapMode = MapModule.MapManager.Street
            }

            Button {
                text: "Спутник"
                Material.background: App.Theme.palette.base2
                Material.foreground: App.Theme.palette.text
                Material.roundedScale: Material.ExtraSmallScale
                onPressed: MapModule.MapManager.mapMode = MapModule.MapManager.Satellite
            }

            Button {
                text: "Гибрид"
                Material.background: App.Theme.palette.base2
                Material.foreground: App.Theme.palette.text
                Material.roundedScale: Material.ExtraSmallScale
                onPressed: MapModule.MapManager.mapMode = MapModule.MapManager.Hybrid
            }

            Button {
                text: "Авто"
                Material.background: App.Theme.palette.base2
                Material.foreground: App.Theme.palette.text
                Material.roundedScale: Material.ExtraSmallScale
                onPressed: MapModule.MapManager.mapMode = MapModule.MapManager.Auto
            }
        }
    }
}