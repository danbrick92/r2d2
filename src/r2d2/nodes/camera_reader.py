from typing import Union

import numpy as np
from cv2 import VideoCapture  # pylint: disable=no-name-in-module

from r2d2.states.camera import Camera
from r2d2.utils.mqtt import MQTTClient


class CameraReader:
    def __init__(
        self,
        mqtt: MQTTClient,
        topic_name: str = "r2d2/camera_read_state",
        cam_index: int = 0,
    ) -> None:
        self.mqttc = mqtt
        self.cam: Union[VideoCapture, None] = None
        self.cam_index = cam_index
        self.topic_name = topic_name

    async def __start_camera(self) -> None:
        if self.cam is None or not self.cam.isOpened():
            self.cam = VideoCapture(self.cam_index)

    def __stop_camera(self) -> None:
        if self.cam is None:
            return
        if self.cam.isOpened():
            self.cam.release()

    async def take_picture(self) -> Union[None, np.ndarray]:
        await self.__start_camera()
        if self.cam is None:
            return None
        ret, frame = self.cam.read()
        if ret:
            return frame
        return None

    async def read(self) -> None:
        # Take picture
        picture = await self.take_picture()
        if picture:
            camera_state = Camera(image=picture)
            self.mqttc.publish(self.topic_name, camera_state.to_json())
        else:
            raise ValueError("Failed to capture image")

    def __del__(self) -> None:
        self.__stop_camera()
