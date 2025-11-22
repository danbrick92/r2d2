from pydantic import BaseModel, Field

from r2d2.states.serial_initializer import SerialInitializer


class MotorState(BaseModel, SerialInitializer):
    forward: bool = Field(..., description="Should it go forward?")
    backward: bool = Field(..., description="Should it go backward?")
    left_speed: int = Field(..., description="How fast the left wheel goes")
    right_speed: int = Field(..., description="How fast the right wheel goes")

    @staticmethod
    def initialize_from_serial(serial_line: str) -> "MotorState":
        try:
            s = serial_line[2:]
            parts = s.split(" ")
            if len(parts) != 4:
                raise ValueError(f"incorrect num parts: {len(parts)}")
            return MotorState(
                forward=bool(int(parts[0])),
                backward=bool(int(parts[1])),
                left_speed=int(parts[2]),
                right_speed=int(parts[3]),
            )
        except Exception as e:
            raise ValueError(f"cannot interpret serial line: {serial_line}") from e
