import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Corona.Gui.Theme
import io.quasar.constellation.network

import "../ui" as UI

Page {
    property var channelNames: [
        "", "АПД", "РЛМ", "ЭВМ", "УМ", "К 5", "КОМ", "НАВ"
    ]

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

    Component.onCompleted: NetworkAPI.powerSwitch.start(NetworkAPI.powerSwitch.configIp, NetworkAPI.powerSwitch.configPort, 10)

    component PowerSwitchStatusEntry : Pane {
        property real max_voltage: NetworkAPI.powerSwitch.configCellMaxVoltages[channel - 1]
        property real min_voltage: NetworkAPI.powerSwitch.configCellMinVoltages[channel - 1]
        property real max_current: 3000
        property real min_current: 0
        required property int channel
        property real voltage: NetworkAPI.powerSwitch.channels[channel].voltage
        property real current: NetworkAPI.powerSwitch.channels[channel].current

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
                value: (current - min_current) / (max_current - min_current)
                size: 40
                lineWidth: 6
                primaryColor: Theme.io.color("green")
                secondaryColor: Theme.io.color("overlay0")
                Layout.alignment: Qt.AlignHCenter
            }
            Label {
                text: NetworkAPI.powerSwitch.configChannelNames[channel - 1]
                font {
                    pointSize: 13
                    weight: Font.Bold
                }
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }
            Switch {
                checked: NetworkAPI.powerSwitch.channels[channel].status
                checkable: false
                display: AbstractButton.IconOnly
                Material.accent: Theme.io.color("overlay0")
                Layout.alignment: Qt.AlignHCenter
                onPressed: NetworkAPI.powerSwitch.toggleChannel(channel)
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
                units: "В"

                // li-io cell count = 5
                //
                // gaugeColorsPercentage: []
                min: 0
                max: NetworkAPI.powerSwitch.configCellMaxVoltage * NetworkAPI.powerSwitch.configCellCount
                value: NetworkAPI.powerSwitch.channels[0].voltage
                Component.onCompleted: console.error(min, max)
            }

            UI.CircularGauge {
                width: 75
                height: 75
                units: "А"

                min: 0
                max: NetworkAPI.powerSwitch.configCellMaxCurrent
                value: NetworkAPI.powerSwitch.channels[0].current / 1000
            }
        }

        Material.background: Theme.io.color("surface0")
        Material.elevation: 200
    }

    RowLayout {
        anchors.fill: parent

        InputGaugesStatus {}

        PowerSwitchStatusEntry {
            channel: 1
        }

        PowerSwitchStatusEntry {
            channel: 2
        }

        PowerSwitchStatusEntry {
            channel: 3
        }

        PowerSwitchStatusEntry {
            channel: 4
        }

        PowerSwitchStatusEntry {
            channel: 5
        }

        PowerSwitchStatusEntry {
            channel: 6
        }

        PowerSwitchStatusEntry {
            channel: 7
        }
    }
}