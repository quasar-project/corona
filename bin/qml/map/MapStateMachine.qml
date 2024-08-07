import QtQuick
import QtQuick.Controls
import QtQml.StateMachine as QtStateMachine

QtStateMachine.StateMachine {
    required property RoundButton rulerButton
    required property RoundButton tileLoaderButton

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

        QtStateMachine.SignalTransition {
            targetState: tileLoaderActive
            signal: tileLoaderButton.clicked
        }

        onEntered: {
            state = "idle"
            console.log("MapStateMachine: " + state)
        }
    }

    QtStateMachine.State {
        id: rulerActive

        QtStateMachine.SignalTransition {
            targetState: idle
            signal: rulerButton.clicked
        }

        onEntered: {
            state = "rulerActive"
            console.log("MapStateMachine: " + state)
        }
    }

    QtStateMachine.State {
        id: tileLoaderActive

        QtStateMachine.SignalTransition {
            targetState: idle
            signal: tileLoaderButton.clicked
        }

        onEntered: {
            state = "tileLoaderActive"
            console.log("MapStateMachine: " + state)
        }
    }

    Component.onCompleted: console.log("MapStateMachine: state machine initialized with default state: <" + state + ">")
}