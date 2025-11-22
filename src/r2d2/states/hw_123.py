from pydantic import BaseModel, Field


class Vector3(BaseModel):
    x: int = Field(..., description="X-axis raw value")
    y: int = Field(..., description="Y-axis raw value")
    z: int = Field(..., description="Z-axis raw value")


class Hw123(BaseModel):
    acceleration: Vector3 = Field(
        ..., description="3-axis acceleration in mg (milligravity units)"
    )
    gyro: Vector3 = Field(
        ..., description="3-axis angular velocity in deg/s (scaled integer)"
    )
    temperature: int = Field(
        ..., description="Raw temperature value; IMU-specific scaling"
    )

    # --- Optional helper properties ---
    @property
    def acceleration_g(self) -> tuple[float, float, float]:
        """
        Convert mg to g units.
        1000 mg = 1 g
        """
        return (
            self.acceleration.x / 1000.0,
            self.acceleration.y / 1000.0,
            self.acceleration.z / 1000.0,
        )

    @property
    def gyro_dps(self) -> tuple[float, float, float]:
        """
        Gyro values are typically scaled integers.
        This assumes ±250°/s range, where 131 LSB = 1°/s
        """
        scale = 131.0
        return (
            self.gyro.x / scale,
            self.gyro.y / scale,
            self.gyro.z / scale,
        )

    @property
    def temperature_c(self) -> float:
        """
        Converts raw temperature to °C using common MPU-style formula:
            T(°C) = (raw / 340) + 36.53
        If your sensor uses a different formula, I can adjust it.
        """
        return (self.temperature / 340.0) + 36.53
