import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtLocation
import QtPositioning
import QtQuick.Layouts

import QtExtensions 1.0
import QtExtensions.Toolkit 1.0
import Quasar 1.0

Map {
    // { 0 - offline, 5 - schema, 4 - hybrid, 1 - satellite }
    property int mapmode: 1

    id: map
    anchors.fill: parent
    plugin: Plugin {
        name: "osm"
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "file:///" + provider.path
        }
    }

    layer {
        enabled: true
        smooth: true
        samples: 8
    }
    center: QtPositioning.coordinate(60, 39.7)
    activeMapType: map.supportedMapTypes[mapmode]
    copyrightsVisible: false
    tilt: 15
    zoomLevel: 14

    Component.onCompleted: {
        QtxGlobal.rootWindow = window_root
        QtxGlobal.root = root
        QtxGlobal.map = map
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
        rotationScale: 1/12
        property: "zoomLevel"
    }
    DragHandler {
        id: drag
        target: null
        onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
    }
    Shortcut {
        enabled: map.zoomLevel < map.maximumZoomLevel
        sequence: StandardKey.ZoomIn
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