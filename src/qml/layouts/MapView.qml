import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtLocation
import QtPositioning
import QtQuick.Layouts

import Corona.Gui.Theme
import Corona.Map.Provider as CoronaMapProvider

import "map" as MapGroup

Map {
    property bool overrideMapMode: false
    property int mapmode: Theme.mode === Theme.Dark ?
          provider.getMapMode(CoronaMapProvider.OpenStreetMapProvider.Satellite)
        : provider.getMapMode(CoronaMapProvider.OpenStreetMapProvider.Scheme)
    function setMapMode(mode) {
        if(overrideMapMode) {
            mapmode = mode
            console.log("map mode changed to: " + mapmode)
        }
        else console.warn("setMapMode is not allowed: overrideMapMode is false")
    }

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
        tileLoaderButton: mapToolbar.tileLoaderButton
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