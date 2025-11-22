from typing import cast

from r2d2.states.base_led_state import BaseLedState


class BackLedState(BaseLedState):
    @staticmethod
    def initialize_from_serial(serial_line: str) -> "BackLedState":
        try:
            base_led = BaseLedState.initialize_from_serial(serial_line)
            return cast(BackLedState, base_led)
        except Exception as e:
            raise ValueError(f"cannot interpret serial line: {serial_line}") from e
