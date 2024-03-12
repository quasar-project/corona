import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Corona.Gui.Theme
import io.quasar.constellation.network

import "../ui" as UI

Page {
    id: control
    title: "Коммутатор питания"
    header: ToolBar {
        height: 26
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            Label {
                text: control.title
                font {
                    capitalization: Font.AllUppercase
                    pointSize: 12
                }
            }
        }
        Material.primary: Theme.io.color("surface1")
    }
    Material.elevation: 200

    component PowerSwitchStatusEntry : Pane {
        required property real voltage
        required property real current
        required property real max_voltage
        required property real min_voltage
        required property int channel

        ColumnLayout {
            Label {
                text: `${Number(voltage).toFixed(1)} В`
                font {
                    pointSize: 13
                    weight: Font.Bold
                }
                color: Theme.io.color("green")
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }
            Label {
                text: `${Number(current).toFixed(0)} мА`
                font {
                    pointSize: 12
                    weight: Font.DemiBold
                }
                color: Theme.io.color("sky")
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            UI.CircularProgressBar {
                value: 0.5
                size: 40
                lineWidth: 6
                primaryColor: Theme.io.color("green")
                secondaryColor: Theme.io.color("overlay0")
                Layout.alignment: Qt.AlignHCenter
            }
            Label {
                text: `CH ${channel}`
                font {
                    pointSize: 13
                    weight: Font.Bold
                }
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }
            Switch {
                checked: true
                display: AbstractButton.IconOnly
                Material.accent: Theme.io.color("overlay0")
                Layout.alignment: Qt.AlignHCenter
                onCheckedChanged: NetworkAPI.powerSwitch.toggleChannel(channel)
            }
        }
        Material.background: Theme.io.color("surface0")
        Material.elevation: 200
    }

    component InputGaugesStatus : Pane {
        ColumnLayout {
            UI.CircularGauge {
                width: 75
                height: 75

                min: 0
                max: 100
                value: 15
            }

            UI.CircularGauge {
                width: 75
                height: 75

                min: 0
                max: 50
                value: 15
            }
        }

        Material.background: Theme.io.color("surface0")
        Material.elevation: 200
    }

    RowLayout {
        anchors.fill: parent

        InputGaugesStatus {

        }

        PowerSwitchStatusEntry {
            voltage: 10.0
            current: 215
            channel: 1
            max_voltage: 12.0
            min_voltage: 8.0
        }

        PowerSwitchStatusEntry {
            voltage: 12.0
            current: 215
            channel: 2
            max_voltage: 12.0
            min_voltage: 8.0
        }

        PowerSwitchStatusEntry {
            voltage: 10.0
            current: 215
            channel: 3
            max_voltage: 12.0
            min_voltage: 8.0
        }

        PowerSwitchStatusEntry {
            voltage: 10.0
            current: 215
            channel: 4
            max_voltage: 12.0
            min_voltage: 8.0
        }

        PowerSwitchStatusEntry {
            voltage: 10.0
            current: 215
            channel: 5
            max_voltage: 12.0
            min_voltage: 8.0
        }

        PowerSwitchStatusEntry {
            voltage: 8.0
            current: 230
            channel: 6
            max_voltage: 12.0
            min_voltage: 8.0
        }

        PowerSwitchStatusEntry {
            voltage: 10.0
            current: 215
            channel: 7
            max_voltage: 12.0
            min_voltage: 8.0
        }
    }
}