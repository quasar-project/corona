import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import QtExtensions.Toolkit 1.0

Popup {
    parent: Overlay.overlay
    width: parent.width / 1.5
    height: parent.height / 1.5
    modal: true
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    visible: true
    clip: true
    padding: 0

    Overlay.modal: Rectangle { color: "#66000000" }

    Page {
        anchors.fill: parent
        padding: 0
        leftInset: 0
        rightInset: 0
        topInset: 0
        bottomInset: 0
        header: ToolBar {

            height: 24
            contentHeight: 24
            RowLayout {
                spacing: 0

                component InlineSeparator : ToolSeparator {
                    Layout.preferredHeight: 24
                    padding: 0
                }
                anchors.fill: parent
                ToolButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 24
                    text: "Имя"
                }
                InlineSeparator {}
                ToolButton {
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 200
                    text: "Дата изменения"
                }
                InlineSeparator {}
                ToolButton {
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 200
                    text: "Тип"
                }
                InlineSeparator {}
                ToolButton {
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 200
                    text: "Размер"
                }
            }
        }

        ListView {
            anchors.fill: parent
            model: XFileDialogModel {}
            delegate: ItemDelegate {
                required property int index
                required property bool isDirectory
                required property string name
                required property string size
                required property date lastChanged
                required property string extension

                width: parent.width
                height: 24

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        Layout.leftMargin: 50
                        Layout.fillWidth: true
                        Layout.preferredHeight: 24
                        text: name
                        font {
                            weight: Font.DemiBold
                        }
                    }

                    Label {
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 230
                        text: lastChanged.toLocaleDateString(Qt.locale("ru_RU"))
                    }

                    Label {
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 220
                        text: isDirectory ? "Папка с файлами"
                            : "Файл \"" + extension + "\""
                    }

                    Label {
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 210
                        text: size
                    }
                }
            }
        }
    }
}