import serial  # type: ignore

from r2d2.serial_readers.base_serial_reader import BaseSerialReader
from r2d2.utils.context import Context


class ArduinoSerialReader(BaseSerialReader):
    def __init__(self, context: Context) -> None:
        super().__init__(context)
        self.port = self.config.arduino_serial_reader.port
        self.baud_rate = self.config.arduino_serial_reader.baud_rate
        self.timeout = self.config.arduino_serial_reader.timeout
        self.arduino = serial.Serial(
            port=self.port, baudrate=self.baud_rate, timeout=self.timeout
        )

    async def read_serial(self) -> str:
        data = self.arduino.read_all()
        return data.decode("utf-8")

    async def catchup(self) -> None:
        self.log(message="Catching up by resetting buffer")
        self.arduino.reset_input_buffer()
