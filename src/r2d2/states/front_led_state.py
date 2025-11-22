from typing import cast

from r2d2.states.base_led_state import BaseLedState


class FrontLedState(BaseLedState):
    @staticmethod
    def initialize_from_serial(serial_line: str) -> "FrontLedState":
        try:
            base_led = BaseLedState.initialize_from_serial(serial_line)
            return cast(FrontLedState, base_led)
        except Exception as e:
            raise ValueError(f"cannot interpret serial line: {serial_line}") from e
