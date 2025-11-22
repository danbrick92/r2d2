from pydantic import BaseModel, Field


class TfLuna(BaseModel):
    distance: int = Field(..., description="Distance in centimeters (cm)")
    strength: int = Field(..., description="Signal strength (unitless ADC value)")
    temperature: int = Field(
        ..., description="Temperature in centi-degrees Celsius (raw)"
    )

    @property
    def temperature_c(self) -> float:
        return self.temperature / 100.0
