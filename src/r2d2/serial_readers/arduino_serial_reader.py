from logging import Logger
import serial  # type: ignore
from r2d2.serial_readers.base_serial_reader import BaseSerialReader


class ArduinoSerialReader(BaseSerialReader):
    def __init__(
        self,
        logger: Logger,
        port: str = "/dev/ttyACM0",
        baud_rate: int = 38400,
        timeout: float = 1.0
    ) -> None:
        super().__init__(logger)
        self.arduino = serial.Serial(port=port, baudrate=baud_rate, timeout=timeout)

    async def read_serial(self) -> str:
        data = self.arduino.read_all()
        return data.decode("utf-8")

    async def catchup(self) -> None:
        self.log(message="Catching up by resetting buffer")
        self.arduino.reset_input_buffer()
