import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

import io.corona.standalone.app as App

MapItemGroup {
    required property Map map
    required property bool active

    function add(mouseX, mouseY) {
        if(!this.active)
            return
        const coord = map.toCoordinate(Qt.point(mouseX, mouseY))
        if(!polyline.path.length)
            polyline.path = [coord]
        else
            polyline.path.push(coord)
        if(polyline.path.length >= 2)
            polyline.referenceSurface = QtLocation.ReferenceSurface.Globe
    }

    function clear() {
        polyline.referenceSurface = QtLocation.ReferenceSurface.Map
        polyline.path = []
    }

    id: group
    visible: this.opacity > 0
    opacity: this.active ? 0.9 : 0
    onActiveChanged: {
        if(!this.active)
            this.clear()
    }

    Behavior on opacity { NumberAnimation { duration: 100; easing.type: Easing.InOutQuad } }

    MapPolyline {
        id: polyline

        line {
            color: App.Theme.palette.yellow
            width: 3
        }
        path: []
        referenceSurface: QtLocation.ReferenceSurface.Map
    }

    MapItemView {
        model: polyline.path
        delegate: MapQuickItem {
            required property var modelData

            anchorPoint {
                x: width / 2
                y: height / 2
            }
            coordinate: this.modelData
            zoomLevel: 0
            sourceItem: Rectangle {
                width: 8
                height: 8
                radius: 4
                color: App.Theme.palette.yellow
            }
        }
    }

    MapQuickItem {
        function distance(path) {
            if(path.length < 2)
                return 0
            let dist = 0
            for(let i = 0; i < path.length - 1; i++)
                dist += path[i].distanceTo(path[i + 1])
            return dist
        }

        function formatMeters(meters) {
            if(meters < 1000)
                return meters.toFixed(0) + " м"
            return (meters / 1000).toFixed(1) + " км"
        }

        id: distanceTooltip
        enabled: polyline.path.length >= 2
        visible: this.enabled
        coordinate: this.enabled ? polyline.path[polyline.path.length - 1] : QtPositioning.coordinate(0, 0)
        anchorPoint {
            x: width / 2
            y: height + 10
        }
        sourceItem: Rectangle {
            width: distanceLabel.width + 16
            height: 20
            radius: 10
            color: App.Theme.palette.yellow

            Label {
                id: distanceLabel
                anchors.centerIn: parent
                text: distanceTooltip.formatMeters(distanceTooltip.distance(polyline.path))
                font.weight: Font.DemiBold
                Material.foreground: App.Theme.palette.base2
            }
        }
    }

    MapQuickItem {
        id: clearButton
        enabled: polyline.path.length >= 2
        visible: this.enabled
        coordinate: this.enabled ? polyline.path[polyline.path.length - 1] : QtPositioning.coordinate(0, 0)
        sourceItem: RoundButton {
            opacity: hovered ? 1 : .2
            radius: 16
            width: 40
            height: 40
            icon.source: "qrc:/resources/icons/close.svg"
            display: AbstractButton.IconOnly

            Material.background: App.Theme.palette.red
            Material.foreground: App.Theme.palette.text
            Material.elevation: 60
            onClicked: group.clear()
        }
    }
}