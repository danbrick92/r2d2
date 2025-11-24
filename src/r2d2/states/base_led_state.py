from pydantic import BaseModel, Field

from r2d2.utils.serial_initializer import SerialInitializer


class BaseLedState(BaseModel, SerialInitializer):
    red: int = Field(..., description="Power of the red led")
    green: int = Field(..., description="Power of the green led")
    blue: int = Field(..., description="Power of the blue led")

    @staticmethod
    def initialize_from_serial(serial_line: str) -> "BaseLedState":
        try:
            s = serial_line[2:]
            parts = s.split(" ")
            if len(parts) != 3:
                raise ValueError(f"incorrect num parts: {len(parts)}")
            return BaseLedState(
                red=int(parts[0]), green=int(parts[1]), blue=int(parts[2])
            )
        except Exception as e:
            raise ValueError(f"cannot interpret serial line: {serial_line}") from e
