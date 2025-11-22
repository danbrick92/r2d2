from pydantic import BaseModel, Field


class StepperState(BaseModel):
    speed: int = Field(..., description="How fast should the head move?")
    direction: int = Field(
        ..., description="How much should it move clockwise/counter?"
    )
