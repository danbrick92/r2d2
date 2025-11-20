from pydantic import BaseModel, Field


class Stepper(BaseModel):
    speed: int = Field(..., description="How fast should the head move?")
    direction: int = Field(
        ..., description="How much should it move clockwise/counter?"
    )
