from typing import Dict, Union
import asyncio
from logging import Logger

from r2d2.nodes.base_node import BaseNode
from r2d2.serial_readers.base_serial_reader import BaseSerialReader
from r2d2.utils.mqtt import MQTTClient
from r2d2.utils.serial_initializer_route import SerialInitializerRoute
from r2d2.config.config import Config
from r2d2.utils.loop_operation import LoopOperationMixin


class ArduinoReaderNode(BaseNode, LoopOperationMixin):
    def __init__(
        self,
        mqtt_client: MQTTClient,
        logger: Logger,
        config: Config,
        serial_reader: BaseSerialReader,
        router: Dict[str, SerialInitializerRoute],
        rate: float
    ) -> None:
        super().__init__(mqtt_client, logger, config)
        self.serial_reader = serial_reader
        self.router = router
        self.serial_reader_task: Union[None, asyncio.Task] = None
        self.rate = rate

    async def init(self) -> None:
        self.log(f"Starting serial reader: {self.serial_reader.whoami()}")
        async with asyncio.TaskGroup():
            self.serial_reader_task = asyncio.create_task(self.serial_reader.start())

    async def run(self) -> None:
        self.log("Running node")
        await self.loop(
            async_func=self._read,
            rate=self.rate,
            catchup=True  # serial queue gets backed up otherwise
        )

    async def cleanup(self) -> None:
        if self.serial_reader_task:
            self.log(f"Cancelling serial reader task: {self.serial_reader.whoami()}")
            self.serial_reader_task.cancel()

    async def catchup(self) -> None:
        await self.serial_reader.catchup()

    async def _read(self) -> None:
        data = await self.serial_reader.read_serial()
        if data.strip() == "":
            return
        lines = data.splitlines()
        for line in lines:
            await self._route_to_topic(line)

    async def _route_to_topic(self, line: str) -> None:
        if line == "":
            return
        s = line[0]

        route = self.router.get(s, None)
        if route is None:
            return

        state = route.state.initialize_from_serial(line)
        state_json = state.model_dump_json()
        self.mqtt_client.publish(route.topic, state_json)
