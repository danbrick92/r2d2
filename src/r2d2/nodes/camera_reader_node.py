from typing import Union
import logging

import numpy as np
from cv2 import VideoCapture  # pylint: disable=no-name-in-module

from r2d2.states.camera_state import CameraState
from r2d2.utils.mqtt import MQTTClient
from r2d2.utils.loop_operation import LoopOperationMixin
from r2d2.nodes.base_node import BaseNode
from r2d2.config.config import Config


class CameraReaderNode(BaseNode, LoopOperationMixin):
    def __init__(
        self,
        mqtt_client: MQTTClient,
        logger: logging.Logger,
        config: Config,
        rate: float,
        cam_index: int = 0,
        topic_name: str = "r2d2/camera_read_state",
    ) -> None:
        super().__init__(
            mqtt_client,
            logger,
            config
        )
        self.rate = rate
        self.cam_index = cam_index
        self.cam: Union[VideoCapture, None] = None
        self.topic_name = topic_name

    async def init(self) -> None:
        await self.__start_camera()

    async def run(self) -> None:
        await self.loop(
            async_func=self._read,
            rate=self.rate,
            catchup=False  # worst that happens is we skip frames
        )

    async def cleanup(self) -> None:
        self.__stop_camera()

    async def __start_camera(self) -> None:
        if self.cam is None or not self.cam.isOpened():
            self.cam = VideoCapture(self.cam_index)

    def __stop_camera(self) -> None:
        if self.cam is None:
            return
        if self.cam.isOpened():
            self.cam.release()

    async def _take_picture(self) -> Union[None, np.ndarray]:
        await self.__start_camera()
        if self.cam is None:
            return None
        ret, frame = self.cam.read()
        if ret:
            return frame
        return None

    async def _read(self) -> None:
        # Take picture
        picture = await self._take_picture()
        if picture:
            camera_state = CameraState(image=picture)
            self.mqtt_client.publish(self.topic_name, camera_state.to_json())
        else:
            self.log(
                message="Failed to capture image",
                details={"cam_index": self.cam_index},
                level=logging.ERROR
            )
            raise ValueError("Failed to capture image")

    def __del__(self) -> None:
        self.__stop_camera()
