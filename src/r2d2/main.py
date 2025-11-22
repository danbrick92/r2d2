import asyncio
from typing import Dict

from r2d2.nodes.arduino_reader import ArduinoReader
from r2d2.serial_readers.arduino_mock_serial_reader import ArduinoMockSerialReader
from r2d2.states.back_led import BackLed
from r2d2.utils.mqtt import MQTTClient
from r2d2.utils.serial_initializer_route import SerialInitializerRoute


def get_router() -> Dict[str, SerialInitializerRoute]:
    return {
        "b": SerialInitializerRoute(state=BackLed, topic="r2d2/back_led_read_state")
    }


async def main():
    serial_reader = ArduinoMockSerialReader()
    router = get_router()
    mqttc = MQTTClient()
    mqttc.connect()
    reader = ArduinoReader(reader=serial_reader, router=router, mqtt=mqttc)
    async with asyncio.TaskGroup():
        asyncio.create_task(serial_reader.start())

        while True:
            await reader.read()
            await asyncio.sleep(0.01)


if __name__ == "__main__":
    asyncio.run(main())
