import QtQuick
import QtQuick.Controls
import QtQml.StateMachine as QtStateMachine

QtStateMachine.StateMachine {
    property Button rulerButton

    property string state: "idle"

    id: stateMachine
    initialState: idle
    running: true

    QtStateMachine.State {
        id: idle

        QtStateMachine.SignalTransition {
            targetState: rulerActive
            signal: rulerButton.pressed
        }

        onEntered:
        {
            state = "idle"
            console.log("state machine: " + state)
        }
    }

    QtStateMachine.State {
        id: rulerActive

        QtStateMachine.SignalTransition {
            targetState: idle
            signal: rulerButton.pressed
        }

        onEntered:
        {
            state = "rulerActive"
            console.log("state machine: " + state)
        }
    }
}