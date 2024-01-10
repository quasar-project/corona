import QtQuick
import QtQuick.Controls
import QtQml.StateMachine as QtStateMachine

QtStateMachine.StateMachine {
    property Button rulerButton

    id: stateMachine
    initialState: idle
    running: true

    QtStateMachine.State {
        id: idle

        QtStateMachine.SignalTransition {
            target: rulerActive
            signal: rulerButton.pressed
        }

        onEntered: console.log("state machine: idle")
    }

    QtStateMachine.State {
        id: rulerActive

        QtStateMachine.SignalTransition {
            target: idle
            signal: rulerButton.pressed
        }

        onEntered: console.log("state machine: rulerActive")
    }
}