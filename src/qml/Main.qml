import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtLocation
import QtPositioning

import QtEx.Geo 1.0

ApplicationWindow {
    id: root

    title: "Corona"
    minimumWidth: 720
    minimumHeight: 360
    width: 1500
    height: 800
    visible: true

    QtExGoogleMapsProvider { id: provider }

    Map {
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
        center: QtPositioning.coordinate(43.7, 39.7)
        activeMapType: map.supportedMapTypes[4]
        zoomLevel: 14
    }
}

