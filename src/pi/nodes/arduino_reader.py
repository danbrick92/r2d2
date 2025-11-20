from typing import Dict
from serial_readers.base_serial_reader import BaseSerialReader
from utils.serial_initializer_route import SerialInitializerRoute
from utils.mqtt import MQTTClient


class ArduinoReader:
    def __init__(
        self,
        reader: BaseSerialReader,
        router: Dict[str, SerialInitializerRoute],
        mqtt: MQTTClient
    ) -> None:
        self.reader = reader
        self.router = router
        self.mqttc = mqtt

    async def read(self) -> None:
        data = await self.reader.read_serial()
        if data.strip() == "":
            return
        lines = data.splitlines()
        for line in lines:
            await self.route_to_topic(line)

    async def route_to_topic(self, line: str) -> None:
        if line == "":
            return
        s = line[0]

        route = self.router.get(s, None)
        if route is None:
            return

        state = route.state.initialize_from_serial(line)
        state_json = state.model_dump_json()
        self.mqttc.publish(route.topic, state_json)
