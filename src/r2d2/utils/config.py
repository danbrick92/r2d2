from typing import Dict

from r2d2.utils.reflection import load_fqcn
from r2d2.utils.serial_initializer import SerialInitializer


class SerialInitializerRoute:
    def __init__(self, topic: str, fqcn: str) -> None:
        self.topic = topic
        self.fqcn = fqcn
        self.state = load_fqcn(self.fqcn, as_instance=False)
        if not issubclass(self.state, SerialInitializer):
            raise ValueError(f"{self.fqcn} is not SerialInitializer")


class Config:
    # Base
    base_rate: float = 0.01

    # MQTT
    mqtt_url: str = "127.0.0.1"
    mqtt_port: int = 1883
    mqtt_keepalive: int = 60

    # Nodes
    class ArduinoReaderNodeConfig:
        router: Dict[str, SerialInitializerRoute] = {
            "b": SerialInitializerRoute(
                topic="r2d2/back_led_read_state",
                fqcn="r2d2.states.back_led_state.BackLedState",
            ),
            "f": SerialInitializerRoute(
                topic="r2d2/front_led_read_state",
                fqcn="r2d2.states.front_led_state.FrontLedState",
            ),
            "h": SerialInitializerRoute(
                topic="r2d2/hw_123_read_state",
                fqcn="r2d2.states.hw_123_state.Hw123State",
            ),
            "m": SerialInitializerRoute(
                topic="r2d2/motor_read_state", fqcn="r2d2.states.motor_state.MotorState"
            ),
            "s": SerialInitializerRoute(
                topic="r2d2/stepper_read_state",
                fqcn="r2d2.states.stepper_state.StepperState",
            ),
            "t": SerialInitializerRoute(
                topic="r2d2/tf_luna_read_state",
                fqcn="r2d2.states.tf_luna_state.TfLunaState",
            ),
        }
        rate: float = 0.05

    arduino_reader_node = ArduinoReaderNodeConfig()

    class CameraReaderNodeConfig:
        rate: float = 0.03
        cam_index: int = 0
        topic: str = "r2d2/camera_read_state"

    camera_reader_node = CameraReaderNodeConfig()

    # Serial Readers
    class ArduinoMockSerialReaderConfig:
        rate: float = 0.05

    arduino_mock_serial_reader = ArduinoMockSerialReaderConfig()

    class ArduinoSerialReaderConfig:
        port: str = "/dev/ttyACM0"
        baud_rate: int = 38400
        timeout: float = 1.0

    arduino_serial_reader = ArduinoSerialReaderConfig()
