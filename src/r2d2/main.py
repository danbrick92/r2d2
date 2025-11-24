import asyncio
from typing import List

from r2d2.nodes.arduino_reader_node import ArduinoReaderNode
from r2d2.nodes.base_node import BaseNode
from r2d2.nodes.camera_reader_node import CameraReaderNode
from r2d2.serial_readers.arduino_mock_serial_reader import ArduinoMockSerialReader
from r2d2.utils.context import Context


def get_nodes(context: Context) -> List[BaseNode]:
    return [
        ArduinoReaderNode(
            context=context, serial_reader=ArduinoMockSerialReader(context)
        ),
        CameraReaderNode(context=context),
    ]


async def main() -> None:
    context = Context()
    nodes = get_nodes(context)

    context.logger.info("Initializing each nodes")
    for n in nodes:
        await n.init()

    try:
        context.logger.info("Running nodes")
        tasks: List[asyncio.Task] = []
        async with asyncio.TaskGroup():
            for n in nodes:
                t = asyncio.create_task(n.run())
                tasks.append(t)

            while True:
                await asyncio.sleep(context.config.base_rate)

    finally:
        context.logger.info("Shutting down")
        for n in nodes:
            await n.cleanup()


if __name__ == "__main__":
    asyncio.run(main())
