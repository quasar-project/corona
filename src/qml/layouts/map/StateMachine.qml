import QtQuick
import QtQuick.Controls
import QtQml.StateMachine as QtStateMachine

QtStateMachine.StateMachine {
    required property RoundButton rulerButton

    property string state: "idle"

    id: stateMachine
    initialState: idle
    running: true

    QtStateMachine.State {
        id: idle

        QtStateMachine.SignalTransition {
            targetState: rulerActive
            signal: rulerButton.clicked
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
            signal: rulerButton.clicked
        }

        onEntered:
        {
            state = "rulerActive"
            console.log("state machine: " + state)
        }
    }
}