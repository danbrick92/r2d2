import json

import numpy as np
from pydantic import BaseModel, Field


class Camera(BaseModel):
    image: np.ndarray = Field(..., description="The captured image")

    def to_json(self) -> str:
        return json.dumps(self.image.tolist())
