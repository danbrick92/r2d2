class Config:
    nodes = {
        "arduino_reader": {
            "fqcn": "r2d2.nodes.arduino_reader_node.ArduinoReaderNode",
            "args": {
                "reader": "",
                "mqtt": ""
            }
        },
        "camera_reader": {
            "fqcn": "r2d2.nodes.camera_reader_node.CameraReaderNode",
            "args": {
                "reader": "",
                "mqtt": ""
            }
        }
    }

    topics = {
        "back_led_read_state" : {
            "name": "r2d2/back_led_read_state",
            "state": "r2d2.states.back_led_state.BackLedState"
        },
        "camera_read_state": {
            "name": "r2d2/camera_read_state",
            "state": "r2d2.states.camera_state.CameraState"
        },
        "front_led_read_state": {
            "name": "r2d2/front_led_read_state",
            "state": "r2d2.states.front_led_state.FrontLedState"
        },
        "hw_123_read_state": {
            "name": "r2d2/hw_123_read_state",
            "state": "r2d2.states.hw_123_state.Hw123State"
        },
        "motor_read_state": {
           "name": "r2d2/motor_read_state",
           "state": "r2d2.states.motor_state.MotorState"
        },
        "stepper_read_state": {
            "name": "r2d2/stepper_read_state",
            "state": "r2d2.states.stepper_state.StepperState"
        },
        "tf_luna_read_state": {
            "name": "r2d2/tf_luna_read_state",
            "state": "r2d2.states.tf_luna_state.TfLunaState"
        }
    }
