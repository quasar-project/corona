import QtQuick
import QtQuick.Controls
import io.corona.standalone.app as App

RoundButton {
    required property string iconPath
    property bool changeOnCheck: true
    property color activeColor: App.Theme.palette.overlay1
    property color baseColor: App.Theme.palette.base2
    property color foregroundColor: App.Theme.palette.text

    radius: 6
    width: 64
    height: 64
    checkable: true
    checked: false
    icon.source: this.iconPath

    Material.background: changeOnCheck ? checked ? this.activeColor : this.baseColor : this.baseColor
    Material.foreground: changeOnCheck ? checked ? this.baseColor : this.foregroundColor : this.foregroundColor
    Material.elevation: 30
}