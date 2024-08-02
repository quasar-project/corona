import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.map.ui as MapUI

Pane {
    required property var mapStateMachine
    property alias rulerButton: __rulerButton
    property alias tileLoaderButton: __tileLoaderButton

    id: control

    RowLayout {
        MapUI.ToolbarButton {
            id: __rulerButton
            mapStateMachine: control.mapStateMachine
            activeColor: App.Theme.palette.yellow
            stateName: "rulerActive"
            iconPath: "qrc:/resources/icons/ruler.svg"
            tooltipOnText: "Измерить расстояние"
            tooltipOffText: "Прекратить измерение расстояния"
        }

        MapUI.ToolbarButton {
            id: __tileLoaderButton
            mapStateMachine: control.mapStateMachine
            activeColor: App.Theme.palette.green
            stateName: "tileLoaderActive"
            iconPath: "qrc:/resources/icons/grid.svg"
            tooltipOnText: "Инструмент загрузки оффлайн-карт"
            tooltipOffText: "Отмена загрузки оффлайн-карт"
        }
    }
}