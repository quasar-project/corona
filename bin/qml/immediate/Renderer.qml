import QtQuick
import QtQuick.Controls
import io.corona.standalone.imgui as ImguiModule

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

    ImguiModule.ImmediateGuiGenericRenderer {
        id: imgui
        objectName: "imgui"
        anchors.fill: parent

        Component.onCompleted: {
            switch(GraphicsInfo.api) {
                case GraphicsInfo.Unknown: this.graphicsInfo.api = "Unknown"; break
                case GraphicsInfo.Software: this.graphicsInfo.api = "Software"; break
                case GraphicsInfo.OpenVG: this.graphicsInfo.api = "OpenVG"; break
                case GraphicsInfo.OpenGL: this.graphicsInfo.api = "OpenGL (preferred)"; break
                case GraphicsInfo.Direct3D11: this.graphicsInfo.api = "Direct3D11"; break
                case GraphicsInfo.Direct3D12: this.graphicsInfo.api = "Direct3D12"; break
                case GraphicsInfo.Vulkan: this.graphicsInfo.api = "Vulkan"; break
                case GraphicsInfo.Metal: this.graphicsInfo.api = "Metal"; break
                case GraphicsInfo.Null: this.graphicsInfo.api = "Null"; break
                default: this.graphicsInfo.api = "Unknown"; break
            }
            this.graphicsInfo.majorVer = GraphicsInfo.majorVersion
            this.graphicsInfo.minorVer = GraphicsInfo.minorVersion
            switch(GraphicsInfo.profile) {
                case GraphicsInfo.OpenGLNoProfile: this.graphicsInfo.profile = "OpenGL No Profile"; break
                case GraphicsInfo.OpenGLCoreProfile: this.graphicsInfo.profile = "OpenGL Core Profile"; break
                case GraphicsInfo.OpenGLCompatibilityProfile: this.graphicsInfo.profile = "OpenGL Compatibility Profile"; break
            }
            switch(GraphicsInfo.renderableType) {
                case GraphicsInfo.SurfaceFormatUnspecified: this.graphicsInfo.renderableType = "Unspecified"; break
                case GraphicsInfo.SurfaceFormatOpenGL: this.graphicsInfo.renderableType = "OpenGL"; break
                case GraphicsInfo.SurfaceFormatOpenGLES: this.graphicsInfo.renderableType = "OpenGLES"; break
            }
            switch(GraphicsInfo.shaderCompilationType) {
                case GraphicsInfo.RuntimeCompilation: this.graphicsInfo.shaderCompilationType = "Runtime Compilation"; break
                case GraphicsInfo.OfflineCompilation: this.graphicsInfo.shaderCompilationType = "Offline Compilation"; break
            }
            switch(GraphicsInfo.shaderSourceType) {
                case GraphicsInfo.ShaderSourceString: this.graphicsInfo.shaderSourceType = "Shader Source String"; break
                case GraphicsInfo.ShaderSourceFile: this.graphicsInfo.shaderSourceType = "Shader Source File"; break
                case GraphicsInfo.ShaderByteCode: this.graphicsInfo.shaderSourceType = "Shader Byte Code"; break
            }
            switch(GraphicsInfo.shaderType) {
                case GraphicsInfo.UnknownShadingLanguage: this.graphicsInfo.shaderType = "Unknown Shading Language"; break
                case GraphicsInfo.GLSL: this.graphicsInfo.shaderType = "GLSL"; break
                case GraphicsInfo.HLSL: this.graphicsInfo.shaderType = "HLSL"; break
                case GraphicsInfo.RhiShader: this.graphicsInfo.shaderType = "RhiShader"; break
            }
        }
    }

    Shortcut {
        sequence: shortcut
        context: Qt.ApplicationShortcut
        onActivated: imgui_frame.toggle()
    }
}