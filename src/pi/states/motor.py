from pydantic import BaseModel, Field


class Motor(BaseModel):
    forward: bool = Field(..., description="Should it go forward?")
    backward: bool = Field(..., description="Should it go backward?")
    left_speed: int = Field(..., description="How fast the left wheel goes")
    right_speed: int = Field(..., description="How fast the right wheel goes")
