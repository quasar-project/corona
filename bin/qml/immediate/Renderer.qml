import QtQuick
import QtQuick.Controls
import io.corona.standalone.imgui as ImguiModule

Rectangle {
    required property string shortcut
    property real max_opacity: 0.85

    function toggle() { this.opacity = this.opacity > 0.5 ? 0 : this.max_opacity }

    id: imgui_frame
    layer.enabled: true
    color: "transparent"
    anchors.fill: parent
    enabled: this.visible
    visible: this.opacity > 0
    opacity: this.max_opacity
    Behavior on opacity { NumberAnimation { duration: 100; easing.type: Easing.InOutQuad } }

    ImguiModule.ImmediateGuiGenericRenderer {
        id: imgui
        objectName: "imgui"
        anchors.fill: parent
    }

    Shortcut {
        sequence: shortcut
        context: Qt.ApplicationShortcut
        onActivated: imgui_frame.toggle()
    }
}