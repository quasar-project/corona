import QtQuick
import QtQuick.Controls
import io.qdebugenv.rendering as M

Rectangle {
    required property string shortcut
    property real max_opacity: 0.85

    function toggle() { this.opacity = this.opacity > 0.5 ? 0 : this.max_opacity }

    id: imgui_frame
    layer.enabled: false
    color: "transparent"
    anchors.fill: parent
    enabled: this.visible
    visible: this.opacity > 0
    opacity: this.max_opacity
    Behavior on opacity { NumberAnimation { duration: 100; easing.type: Easing.InOutQuad } }
    onVisibleChanged: forceActiveFocus()

    M.ExtendableRenderer {
        id: imgui
        objectName: "imgui"
        anchors.fill: parent
        graphicsInfo: GraphicsInfo
    }

    Shortcut {
        sequence: shortcut
        context: Qt.ApplicationShortcut
        onActivated: imgui_frame.toggle()
    }
}