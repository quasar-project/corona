import QtQuick
import QtQuick.Window
import QtPositioning
import QtLocation

//import io.qdebugenv.rendering 1.0 as QDE_Rendering

Window {
    height: 800
    title: "Hello World"
    visible: true
    width: 1000

    Map {
        id: map

        activeMapType: map.supportedMapTypes[1]
        anchors.fill: parent
        center: QtPositioning.coordinate(60, 39.7)
        copyrightsVisible: false
        tilt: 15
        zoomLevel: 14

        Behavior on center {
            CoordinateAnimation {
                duration: 250
                easing.type: Easing.InOutQuad
            }
        }
        plugin: Plugin {
            name: "qnx"

            PluginParameter {
                name: "qnx.mapping.targetConfigDirectory"
                value: applicationDirPath + "/config"
            }
        }
        Behavior on zoomLevel {
            NumberAnimation {
                duration: 250
                easing.type: Easing.InOutCubic
            }
        }

        layer {
            enabled: true
            samples: 8
            smooth: true
        }
        PinchHandler {
            id: pinch

            grabPermissions: PointerHandler.TakeOverForbidden
            target: null

            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false);
            }
            onRotationChanged: delta => {
                map.bearing -= delta;
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position);
            }
            onScaleChanged: delta => {
                map.zoomLevel += Math.log2(delta);
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position);
            }
        }
        WheelHandler {
            id: wheel

            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland" ? PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Mouse
            property: "zoomLevel"
            rotationScale: 1 / 12
        }
        DragHandler {
            id: drag

            target: null

            onTranslationChanged: delta => map.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequences: [StandardKey.ZoomIn]

            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }
        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut

            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }
    }
}