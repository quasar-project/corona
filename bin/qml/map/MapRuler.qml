import QtQuick
import QtLocation
import QtPositioning

import io.corona.standalone.app as App

MapItemGroup {
    required property Map map
    required property bool active
    property var lastClickedCoordinate: QtPositioning.coordinate(0, 0)

    function add(mouseX, mouseY) {
        if(!this.active)
            return
        lastClickedCoordinate = map.toCoordinate(Qt.point(mouseX, mouseY))
        if(!polyline.path.length)
            polyline.path = [lastClickedCoordinate]
        else
            polyline.path.push(lastClickedCoordinate)
    }

    function clear() {
        polyline.path = []
        lastClickedCoordinate = QtPositioning.coordinate(0, 0)
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
        referenceSurface: path.length >= 2 ? QtLocation.ReferenceSurface.Globe : QtLocation.ReferenceSurface.Map
    }
}