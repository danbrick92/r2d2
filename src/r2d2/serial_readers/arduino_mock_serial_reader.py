import asyncio
import time
from copy import deepcopy
from typing import Dict


class ArduinoMockSerialReader:

    def __init__(self) -> None:
        super().__init__()
        self.rate = 0.5
        self.queue: Dict[float, str] = {}

    async def start(self) -> None:
        while True:
            current_time = time.time()
            data = self.__mock_read()
            self.queue[current_time] = data
            await asyncio.sleep(self.rate)

    def __mock_frontled_read(self) -> str:
        return "fr100 100 100\n"

    def __mock_backled_read(self) -> str:
        return "br0 0 255\n"

    def __mock_tfluna_read(self) -> str:
        return "tr393 1122 4900\n"

    def __mock_hw123_read(self) -> str:
        return "hr1488 -16264 3056 -32 182 83 -4464\n"

    def __mock_motor_read(self) -> str:
        return "mr0 0 0 0\n"

    def __mock_stepper_read(self) -> str:
        return "sr10 0\n"

    def __mock_read(self) -> str:
        serial = ""
        serial += self.__mock_frontled_read()
        serial += self.__mock_backled_read()
        serial += self.__mock_tfluna_read()
        serial += self.__mock_hw123_read()
        serial += self.__mock_motor_read()
        serial += self.__mock_stepper_read()
        return serial

    async def read_serial(self) -> str:
        data = ""
        queue = deepcopy(self.queue)
        for k, v in queue.items():
            data += v
            del self.queue[k]
        return data
