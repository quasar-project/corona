import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.theme as ThemeModule
import io.corona.standalone.map as MapModule
import io.qdebugenv.rendering as QDebugEnv_Rendering

Map {
    required property QDebugEnv_Rendering.ImmediateGUIRenderingFacility imguiRenderer
    property var mapType: MapModule.MapManager.Auto

    function mapTypeBinding(themeMode) {
        if(this.mapType === MapModule.MapManager.Auto)
            return themeMode === App.Theme.Dark
                ? this.supportedMapTypes[MapModule.MapManager.Hybrid]
                : this.supportedMapTypes[MapModule.MapManager.Street]
        else return this.supportedMapTypes[this.mapType]
    }

    id: map
    layer {
        enabled: true
        smooth: true
        samples: 8
    }
    center: QtPositioning.coordinate(60, 39.7)
    activeMapType: this.mapTypeBinding(App.Theme.mode)
    copyrightsVisible: false
    tilt: 15
    zoomLevel: 14
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
}