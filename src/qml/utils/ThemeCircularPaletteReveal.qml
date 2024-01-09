import QtQuick
import QtQuick.Controls
import Corona.Gui.Theme

Loader {
    required property Item rootItem

    id: loader_reveal
    anchors.fill: parent

    function distance(x1, y1, x2, y2){ return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) }

    function toggle(button)
    {
        loader_reveal.sourceComponent = __reveal
        let target = rootItem
        let pos = button.mapToItem(target, 0, 0)
        let mouseX = pos.x
        let mouseY = pos.y
        let radius = Math.max(distance(mouseX, mouseY, 0, 0), distance(mouseX, mouseY, target.width, 0),
            distance(mouseX, mouseY, 0, target.height), distance(mouseX, mouseY, target.width, target.height))
        let reveal = loader_reveal.item
        reveal.start(reveal.width * Screen.devicePixelRatio, reveal.height * Screen.devicePixelRatio, Qt.point(mouseX, mouseY), radius)
    }

    Component {
        id: __reveal

        ThemeCircularPaletteRevealInternal {
            id: reveal
            target: rootItem
            anchors.fill: parent
            onAnimationFinished: loader_reveal.sourceComponent = undefined
            onImageChanged: Theme.toggle()
        }
    }
}