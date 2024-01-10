import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtLocation
import QtPositioning
import QtQuick.Layouts

import Corona.Map.Provider as CoronaMapProvider

import "map" as MapGroup

Map {
    property int mapmode: provider.getMapMode(CoronaMapProvider.OpenStreetMapProvider.Satellite)

    id: map
    anchors.fill: parent
    plugin: Plugin {
        name: "osm"
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "file:///" + provider.directory
        }
    }

    layer {
        enabled: true
        smooth: true
        samples: 8
    }
    Component.onCompleted: console.log(mapmode)
    center: QtPositioning.coordinate(60, 39.7)
    activeMapType: map.supportedMapTypes[mapmode]
    copyrightsVisible: false
    tilt: 15
    zoomLevel: 14


    Behavior on center { CoordinateAnimation { duration: 250; easing.type: Easing.InOutQuad } }
    Behavior on zoomLevel { NumberAnimation { duration: 250; easing.type: Easing.InOutCubic } }

    CoronaMapProvider.OpenStreetMapProvider { id: provider }

    MapGroup.StateMachine {
        id: mapStateMachine
        rulerButton: mapToolbar.rulerButton
    }

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

    MapGroup.ToolBar {
        id: mapToolbar
        anchors {
            top: parent.top
            left: parent.left
            margins: 15
        }
    }
}