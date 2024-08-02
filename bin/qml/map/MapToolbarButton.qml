import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import io.corona.standalone.app as App
import io.corona.standalone.ui as UI

UI.SimpleCheckableToolButton {
    required property var mapStateMachine
    required property string stateName
    required property string tooltipOnText
    required property string tooltipOffText

    Layout.preferredWidth: 48
    Layout.preferredHeight: 48
    enabled: mapStateMachine.state === "idle" || mapStateMachine.state === stateName
    checked: mapStateMachine.state === stateName

    ToolTip.text: checked ? tooltipOffText : tooltipOnText
    ToolTip.delay: 300
    ToolTip.visible: hovered
}