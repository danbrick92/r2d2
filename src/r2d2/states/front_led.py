from pydantic import BaseModel, Field


class FrontLed(BaseModel):
    red: int = Field(..., description="Power of the red led")
    green: int = Field(..., description="Power of the green led")
    blue: int = Field(..., description="Power of the blue led")
