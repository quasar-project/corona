import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Corona.Gui.Theme

Pane {
    // export aliases for state machine
    property alias rulerButton: __rulerButton
    property alias tileLoaderButton: __tileLoaderButton

    id: control

    component StateMachineLinkedButton : RoundButton {
        required property string activeColorName
        required property string stateName
        required property string iconPath
        required property string tooltipOnText
        required property string tooltipOffText

        id: __ctrl
        Layout.preferredWidth: 48
        Layout.preferredHeight: 48
        Material.background: checked ? Theme.io.color(activeColorName) : Theme.io.color("base")
        Material.foreground: checked ? Theme.io.color("background") : Theme.io.color("foreground")
        enabled: mapStateMachine.state === "idle" || mapStateMachine.state === stateName
        display: AbstractButton.IconOnly
        icon.source: iconPath
        radius: 4
        checkable: true
        checked: mapStateMachine.state === stateName

        ToolTip.text: checked ? tooltipOffText : tooltipOnText
        ToolTip.delay: 300
        ToolTip.visible: hovered
    }

    RowLayout {
        StateMachineLinkedButton {
            id: __rulerButton
            activeColorName: "yellow"
            stateName: "rulerActive"
            iconPath: "qrc:/icons/toolbar/ruler.svg"
            tooltipOnText: "Измерить расстояние"
            tooltipOffText: "Прекратить измерение расстояния"
        }

        StateMachineLinkedButton {
            id: __tileLoaderButton
            activeColorName: "green"
            stateName: "tileLoaderActive"
            iconPath: "qrc:/icons/toolbar/grid.svg"
            tooltipOnText: "Инструмент загрузки оффлайн-карт"
            tooltipOffText: "Отмена загрузки оффлайн-карт"
        }
    }
}