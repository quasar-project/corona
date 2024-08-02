import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.ui as UI
import io.corona.standalone.map.ui as MapUI
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
        id: mapMouseArea
        hoverEnabled: true
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
    }

    MapModule.CoordinateTooltip {
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        map: map
        mouseArea: mapMouseArea
    }

    MapUI.StateMachine {
        id: mapStateMachine
        rulerButton: mapToolbar.rulerButton
        tileLoaderButton: mapToolbar.tileLoaderButton
    }

    MapUI.Toolbar {
        id: mapToolbar
        mapStateMachine: mapStateMachine
        anchors {
            top: parent.top
            left: parent.left
            margins: 15
        }
    }

    MapUI.MapModeButton {
        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 12
        }
    }
}