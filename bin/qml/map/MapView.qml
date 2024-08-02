import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.ui as UI
import io.corona.standalone.theme as ThemeModule
import io.corona.standalone.map as MapModule
import io.corona.rendering.immediate as RenderingModule

Map {
    required property RenderingModule.ImmediateGUIRenderingFacility imguiRenderer

    function mapTypeBinding(themeMode) {
        if(MapModule.MapManager.mapMode === MapModule.MapManager.Auto)
            return themeMode === App.Theme.Dark
                ? this.supportedMapTypes[MapModule.MapManager.Hybrid]
                : this.supportedMapTypes[MapModule.MapManager.Street]
        else return this.supportedMapTypes[MapModule.MapManager.mapMode]
    }

    id: map
    layer {
        enabled: true
        smooth: true
        samples: 8
    }
    center: MapModule.MapManager.state.coordinate
    activeMapType: this.mapTypeBinding(App.Theme.mode)
    copyrightsVisible: false
    tilt: 15
    zoomLevel: MapModule.MapManager.state.zoomLevel
    plugin: Plugin {
        name: "cgs"
        PluginParameter {
            name: "cgs.mapping.targetConfigDirectory"
            value: MapModule.MapManager.configPath
        }

        PluginParameter {
            name: "cgs.mapping.targetCacheDirectory"
            value: App.Directories.cache + "/geoservice"
        }
    }

    Component.onDestruction: {
        MapModule.MapManager.state.coordinate = map.center
        MapModule.MapManager.state.zoomLevel = map.zoomLevel
    }


    Behavior on center { CoordinateAnimation { duration: 250; easing.type: Easing.InOutQuad } }
    Behavior on zoomLevel { NumberAnimation { duration: 250; easing.type: Easing.InOutCubic } }

    PinchHandler {
        id: pinch
        target: null
        onActiveChanged: if (active) {
            map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
        }
        onScaleChanged: (delta) => {
            map.zoomLevel += Math.log2(delta)
            map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
        }
        onRotationChanged: (delta) => {
            map.bearing -= delta
            map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
        }
        grabPermissions: PointerHandler.TakeOverForbidden
    }

    WheelHandler {
        id: wheel
        acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
            ? PointerDevice.Mouse | PointerDevice.TouchPad
            : PointerDevice.Mouse
        rotationScale: 1 / 240
        property: "zoomLevel"
    }

    DragHandler {
        enabled: !map.imguiRenderer.visible
        id: drag
        target: null
        onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
    }

    Shortcut {
        enabled: map.zoomLevel < map.maximumZoomLevel
        sequences: [ StandardKey.ZoomIn ]
        onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
    }

    Shortcut {
        enabled: map.zoomLevel > map.minimumZoomLevel
        sequence: StandardKey.ZoomOut
        onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
    }

    MouseArea {
        id: map_mouse_area
        hoverEnabled: true
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
    }

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