import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.theme as ThemeModule
import io.qdebugenv.rendering as QDebugEnv_Rendering

Map {
    required property QDebugEnv_Rendering.ImmediateGUIRenderingFacility imguiRenderer
    property int mapType: 10 // todo: cpp enumeration

    function evalMapType() {
        if(this.mapType === 10)
            return App.Theme.mode === App.Theme.Dark
                ? this.supportedMapTypes[2]
                : this.supportedMapTypes[0]
        else return this.supportedMapTypes[this.mapType]
    }

    id: map
    layer {
        enabled: true
        smooth: true
        samples: 8
    }
    center: QtPositioning.coordinate(60, 39.7)
    activeMapType: this.evalMapType()
    copyrightsVisible: false
    tilt: 15
    zoomLevel: 14
    plugin: Plugin {
        name: "cgs"
        PluginParameter {
            name: "cgs.mapping.targetConfigDirectory"
            value: App.Directories.config + "/geoservice"
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