from pydantic import BaseModel, Field

from r2d2.states.serial_initializer import SerialInitializer


class StepperState(BaseModel, SerialInitializer):
    speed: int = Field(..., description="How fast should the head move?")
    direction: int = Field(
        ..., description="How much should it move clockwise/counter?"
    )

    @staticmethod
    def initialize_from_serial(serial_line: str) -> "StepperState":
        try:
            s = serial_line[2:]
            parts = s.split(" ")
            if len(parts) != 2:
                raise ValueError(f"incorrect num parts: {len(parts)}")
            return StepperState(speed=int(parts[0]), direction=int(parts[1]))
        except Exception as e:
            raise ValueError(f"cannot interpret serial line: {serial_line}") from e
