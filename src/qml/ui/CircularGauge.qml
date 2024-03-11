import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Corona.Gui.Theme

Rectangle {
    id: control

    radius: width / 2
    color: Theme.io.color("crust")

    property real min: 0
    property real max: 100
    property real value: 0

    property var gaugeColorsPercentage: [ 0.15, 0.70 ] // green is implicit (total - (r + y))

    property real angleStart: -50
    property real angleEnd: 230

    function normalize(val, s, e) { return (val - s) / (e - s) }
    function lerp(from, to, t) { return from + (to - from) * t }
    function deg2rad(degrees) { return degrees * Math.PI / 180.0 }

    Canvas {
        id: gauges

        anchors.fill: parent
        onPaint: {
            let ctx = getContext("2d");
            ctx.lineCap='round';
            ctx.clearRect(0, 0, gauges.width, gauges.height);
            ctx.beginPath();
            ctx.strokeStyle = Theme.io.color("red")
            ctx.fillStyle = Theme.io.color("red")
            ctx.lineWidth = 6;

            let total_length = Math.abs(control.angleStart) + Math.abs(control.angleEnd)
            let red_angle = total_length
                * control.gaugeColorsPercentage[0]
                + control.angleStart + 180
            let yellow_angle = total_length
                * (control.gaugeColorsPercentage[1] + control.gaugeColorsPercentage[0])
                + control.angleStart + 180
            ctx.arc(
                gauges.width / 2,
                gauges.height / 2,
                gauges.width / 2.2,
                deg2rad(180 + control.angleStart),
                deg2rad(red_angle)
            );

            ctx.stroke();
            ctx.strokeStyle = Theme.io.color("yellow")
            ctx.fillStyle = Theme.io.color("yellow")
            ctx.beginPath();
            ctx.arc(
                gauges.width / 2,
                gauges.height / 2,
                gauges.width / 2.2,
                deg2rad(red_angle),
                deg2rad(yellow_angle)
            );

            ctx.stroke();
            ctx.strokeStyle = Theme.io.color("green")
            ctx.fillStyle = Theme.io.color("green")
            ctx.beginPath();
            ctx.arc(
                gauges.width / 2,
                gauges.height / 2,
                gauges.width / 2.2,
                deg2rad(yellow_angle),
                deg2rad(180 + control.angleEnd)
            );
            ctx.stroke();
        }
    }

    Text {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.verticalCenter
            topMargin: 0
        }

        font.weight: Font.Bold
        horizontalAlignment: Text.AlignHCenter
        text: Number(control.value).toFixed(2)
        color: Theme.io.color("surface1")
    }

    Rectangle {
        id: arrow
        color: Theme.io.color("text")
        width: control.width / 2
        height: control.height / 20
        radius: height / 2
        transformOrigin: Item.Right
        property real a: control.value
        rotation: control.lerp(control.angleStart, control.angleEnd, control.normalize(control.value, control.min, control.max))

        anchors {
            right: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
    }

    Behavior on value { NumberAnimation { duration: 100; easing.type: Easing.InOutQuad }}
    SequentialAnimation on value {
        NumberAnimation {
            from: 0
            to: 100
            duration: 2000
        }

        NumberAnimation {
            from: 100
            to: 0
            duration: 1000
        }

        loops: Animation.Infinite
    }
}