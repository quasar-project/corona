import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning

import io.corona.standalone.app as App

Menu {
    required property Map map
    property var mouseCoordinates: Qt.point(0, 0)

    id: mapContextMenu
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    MenuItem {
        property var coordinates: map.toCoordinate(mapContextMenu.mouseCoordinates)
        function formatCoordinates(lat, lon) {
            return Number(Math.abs(lat)).toFixed(6) + "°" + (x > 0 ? "E" : "W") + ", " +
                Number(Math.abs(lon)).toFixed(6) + "°" + (y > 0 ? "N" : "S")
        }

        id: cursorCoordinates
        text: formatCoordinates(this.coordinates.latitude, this.coordinates.longitude)
        font.weight: Font.Bold
        onTriggered: App.Utils.copyToClipboard(this.text)

        ToolTip.text: "Нажмите, чтобы скопировать координаты в буфер обмена"
        ToolTip.delay: 300
        ToolTip.visible: hovered
    }

    Action {
        id: zoomIn
        text: "Увеличить"
        onTriggered: map.zoomLevel = Math.round(map.zoomLevel + 1)
    }

    Action {
        id: zoomOut
        text: "Уменьшить"
        onTriggered: map.zoomLevel = Math.round(map.zoomLevel - 1)
    }
}