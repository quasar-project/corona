import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtLocation
import QtPositioning
import QtQuick.Layouts

import QtExtensions 1.0
import QtExtensions.Toolkit 1.0
import Quasar 1.0

ApplicationWindow {
    id: window_root

    title: "Corona"
    minimumWidth: 720
    minimumHeight: 360
    width: 1500
    height: 800
    visible: true
    font.family: mainfont

    Universal.theme: Theme.darkMode === Theme.Dark ? Universal.Dark : Universal.Light
    Universal.background: Theme.io.color(Theme.Base)
    Universal.foreground: Theme.io.color(Theme.Text)
    Universal.accent: Theme.io.color(Theme.Mauve)

    XOsmProvider { id: provider }

    property string mainfont: __fontmain.name
    property string monofont: __fontmono.name
    FontLoader { id: __fontmain; source: "qrc:/fonts/Roboto-Regular.ttf" }
    FontLoader { id: __fontmono; source: "qrc:/fonts/Inconsolata.ttf" }

    Item {
        id: root
        anchors.fill: parent

        Map {
            // { 0 - offline, 5 - schema, 4 - hybrid, 1 - satellite }
            property int mapmode: 4

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

        QtxFileDialog {
            id: __fileDialog
            anchors.centerIn: parent
        }

        Pane {
            anchors.fill: parent
            ColumnLayout {
                Button { text: "Тестовая кнопка" }
                Button { text: "Show dialog"; onPressed: __fileDialog.visible = true }
                CheckBox { text: "Toggle theme"; onPressed: {
                    themeChanger.toggleTheme(this)
                }}
                Pane {
                    ColumnLayout {
                        RowLayout {
                            spacing: 10
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton {}
                            RoundButton { text: "Theme change test" }
                        }
                        RowLayout {
                            RoundButton { text: "QStart"; onPressed: {
                                Quasar.telemetrySocket.verifyChecksum = false
                                Quasar.telemetrySocket.start("192.168.0.5:4554", "192.168.0.7:9955")
                            }
                            }
                            RoundButton { text: "QStop"; onPressed: {
                                Quasar.telemetrySocket.stop()
                            }
                            }
                        }
                        RowLayout {
                            FUIButton {
                                text: "asdasdasd"
                            }
                            FUIButton {
                                enabled: false
                                text: "asdasdasd"
                            }
                            FUISwitch {
                                text: "asdasdasd"
                                contentDescription: "asdasda"
                            }
                        }
                    }
                }
            }
        }
    }

    QtxThemeChanger { id: themeChanger; rootItem: root }
}

