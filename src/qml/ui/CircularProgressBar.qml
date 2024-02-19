import QtQuick

Item {
    id: control

    property int size: 150
    property int lineWidth: 5
    property real value: 0
    property color primaryColor: "#29b6f6"
    property color secondaryColor: "#e0e0e0"
    property int animationDuration: 1000

    width: size
    height: size

    onValueChanged: canvas.degree = value * 360;

    Canvas {
        id: canvas
        property real degree: 0
        anchors.fill: parent
        antialiasing: true

        onDegreeChanged: requestPaint();

        onPaint: {
            var ctx = getContext("2d");

            var x = control.width/2;
            var y = control.height/2;

            var radius = control.size/2 - control.lineWidth
            var startAngle = (Math.PI/180) * 270;
            var fullAngle = (Math.PI/180) * (270 + 360);
            var progressAngle = (Math.PI/180) * (270 + degree);

            ctx.reset()

            ctx.lineCap = 'round';
            ctx.lineWidth = control.lineWidth;

            ctx.beginPath();
            ctx.arc(x, y, radius, startAngle, fullAngle);
            ctx.strokeStyle = control.secondaryColor;
            ctx.stroke();

            ctx.beginPath();
            ctx.arc(x, y, radius, startAngle, progressAngle);
            ctx.strokeStyle = control.primaryColor;
            ctx.stroke();
        }

        Behavior on degree { NumberAnimation { duration: control.animationDuration } }
    }
}