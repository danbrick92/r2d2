from pydantic import BaseModel, Field

from r2d2.utils.serial_initializer import SerialInitializer


class TfLunaState(BaseModel, SerialInitializer):
    distance: int = Field(..., description="Distance in centimeters (cm)")
    strength: int = Field(..., description="Signal strength (unitless ADC value)")
    temperature: int = Field(
        ..., description="Temperature in centi-degrees Celsius (raw)"
    )

    @property
    def temperature_c(self) -> float:
        return self.temperature / 100.0

    # pylint: disable=duplicate-code
    @staticmethod
    def initialize_from_serial(serial_line: str) -> "TfLunaState":
        try:
            s = serial_line[2:]
            parts = s.split(" ")
            if len(parts) != 3:
                raise ValueError(f"incorrect num parts: {len(parts)}")
            return TfLunaState(
                distance=int(parts[0]),
                strength=int(parts[1]),
                temperature=int(parts[2]),
            )
        except Exception as e:
            raise ValueError(f"cannot interpret serial line: {serial_line}") from e
