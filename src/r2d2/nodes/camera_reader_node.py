import asyncio
import subprocess
from typing import Any

import numpy as np

from r2d2.nodes.base_node import BaseNode
from r2d2.states.camera_state import CameraState
from r2d2.utils.context import Context


class CameraReaderNode(BaseNode):
    def __init__(
        self,
        context: Context,
    ) -> None:
        super().__init__(context)
        self.rate = self.config.camera_reader_node.rate
        self.cam_index = self.config.camera_reader_node.cam_index
        self.cam: Any = None
        self.topic = self.config.camera_reader_node.topic
        self.cmd = [
            "ffmpeg",
            "-f",
            "avfoundation",
            "-framerate",
            "30",
            "-video_size",
            "1280x720",
            "-i",
            f"{self.cam_index}:",  # camera index
            "-pix_fmt",
            "rgb24",
            "-vcodec",
            "rawvideo",
            "-f",
            "rawvideo",
            "-",
        ]

    async def init(self) -> None:
        self.log("Starting node")
        await self.__start_camera()
        await self.context.mqtt_manager.connect_mqtt_client(self.mqtt_client)
        self.mqtt_client.loop_start()  # start network loop in background

    async def run(self) -> None:
        self.log("Running node")
        with subprocess.Popen(
            self.cmd, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL
        ) as proc:
            while True:
                raw = proc.stdout.read(1280 * 720 * 3)  # type: ignore
                if not raw:
                    break

                frame = np.frombuffer(raw, dtype=np.uint8)
                frame = frame.reshape((720, 1280, 3))

                camera_state = CameraState(image=frame)
                camera_state_json = camera_state.to_json()
                self.mqtt_client.publish(self.topic, camera_state_json)
                await asyncio.sleep(.001)

    async def cleanup(self) -> None:
        self.log("Stopping node")
        self.__stop_camera()
        try:
            self.mqtt_client.loop_stop()
        except Exception:  # pylint: disable=broad-except
            pass

    async def __start_camera(self) -> None:
        pass
        # if self.cam is None:
        #     self.cam = av.open(
        #         f"{self.cam_index}:",
        #         format="avfoundation",
        #         options={"pixel_format": "uyvy422"},
        #     )

    def __stop_camera(self) -> None:
        pass
        # if self.cam is None:
        #     return
        # self.cam.close()

    def __del__(self) -> None:
        self.__stop_camera()
