import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Corona.Gui.Theme

Pane {
    property alias rulerButton: __rulerButton

    id: control

    RowLayout {
        RoundButton {
            id: __rulerButton
            Layout.preferredWidth: 48
            Layout.preferredHeight: 48
            Material.background: checked ? Theme.io.color("yellow") : Theme.io.color("base")
            Material.foreground: checked ? Theme.io.color("background") : Theme.io.color("foreground")
            display: AbstractButton.IconOnly
            icon.source: "qrc:/icons/toolbar/ruler.svg"
            radius: 4
            checkable: true
            checked: mapStateMachine.state == "rulerActive"
        }
    }
}