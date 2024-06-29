import QtQuick
import QtQuick.Controls
import QtQuick.Window

import io.corona.standalone.theme as M

Loader {
    required property Item rootItem
    required property M.ThemeWrapper theme

    id: loader_reveal
    anchors.fill: parent
    z: 65535

    function distance(x1, y1, x2, y2) { return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) }

    function toggle(button)
    {
        loader_reveal.sourceComponent = __reveal
        var target = rootItem
        var pos = button.mapToItem(target, 0, 0)
        var mouseX = pos.x
        var mouseY = pos.y
        var radius = Math.max(distance(mouseX, mouseY, 0, 0), distance(mouseX, mouseY, target.width, 0),
            distance(mouseX, mouseY, 0, target.height), distance(mouseX, mouseY, target.width, target.height))
        var reveal = loader_reveal.item
        reveal.start(reveal.width * Screen.devicePixelRatio, reveal.height * Screen.devicePixelRatio, Qt.point(mouseX, mouseY), radius)
    }

    Component {
        id: __reveal

        M.CircularReveal {
            id: reveal
            target: rootItem
            anchors.fill: parent
            onAnimationFinished: loader_reveal.sourceComponent = undefined
            onImageChanged: theme.swapMode()
        }
    }
}
