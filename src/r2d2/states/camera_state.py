import json

import numpy as np
from pydantic import BaseModel, Field


class CameraState(BaseModel):
    image: np.ndarray = Field(..., description="The captured image")

    def to_json(self) -> str:
        return json.dumps(self.image.tolist())  # pylint: disable=no-member

    @staticmethod
    def from_json(model_json: str) -> "CameraState":
        return CameraState(image=np.array(json.loads(model_json)))
