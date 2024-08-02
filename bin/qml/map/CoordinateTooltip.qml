import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning

import io.corona.standalone.app as App

Pane {
    required property Map map
    required property MouseArea mouseArea
    property real mouseX: this.mouseArea.mouseX
    property real mouseY: this.mouseArea.mouseY
    property var cursorCoordinate: map.toCoordinate(Qt.point(this.mouseX, this.mouseY))
    property string __latSuffix: this.cursorCoordinate.latitude > 0 ? "N" : "S"
    property string __lonSuffix: this.cursorCoordinate.longitude > 0 ? "E" : "W"

    RowLayout {
        Text {
            color: App.Theme.palette.subtext0
            font {
                weight: Font.DemiBold
                pixelSize: 13
            }

            text: "Широта:"
        }

        Text {
            Layout.preferredWidth: 85
            color: App.Theme.palette.text
            text: Number(Math.abs(cursorCoordinate.latitude)).toFixed(6) + "°" + __latSuffix

            font {
                pixelSize: 13
                weight: Font.ExtraBold
            }
        }

        Text {
            color: App.Theme.palette.subtext0
            text: "Долгота:"

            font {
                pixelSize: 13
                weight: Font.DemiBold
            }
        }

        Text {
            Layout.preferredWidth: 85
            color: App.Theme.palette.text
            text: Number(Math.abs(cursorCoordinate.longitude)).toFixed(6) + "°" + __lonSuffix

            font {
                pixelSize: 13
                weight: Font.ExtraBold
            }
        }
    }
}