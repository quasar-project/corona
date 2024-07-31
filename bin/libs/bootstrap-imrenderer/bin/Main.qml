import QtQuick
import QtQuick.Window
import io.qdebugenv.rendering 1.0 as QDE_Rendering

Window {
    width: 300
    height: 300
    visible: true
    title: "Hello World"

    Rectangle {
        id: rotatingBox
        anchors.centerIn: parent
        width: 300
        height: 300
        color: "lightsteelblue"
        RotationAnimator {
            target: rotatingBox;
            from: 0;
            to: 360;
            duration: 1000
            running: true
            loops: Animation.Infinite
        }
    }
    QDE_Rendering.ImmediateGUIRenderingFacility { shortcut: "\\"; opacity: 0 }
}