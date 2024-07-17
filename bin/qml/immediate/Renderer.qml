import QtQuick
import QtQuick.Controls
import io.corona.standalone.imgui as ImguiModule

Rectangle {
    required property string shortcut

    id: imgui_frame
    layer.enabled: true
    color: "transparent"
    anchors.fill: parent
    enabled: this.visible
    visible: this.opacity > 0
    opacity: 0
    Behavior on opacity { NumberAnimation { duration: 200; easing.type: Easing.InOutCubic } }

    function toggle() {
        this.opacity = this.opacity > 0.5 ? 0 : 1
    }
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