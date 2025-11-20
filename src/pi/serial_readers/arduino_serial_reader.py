import serial
from serial_readers.base_serial_reader import BaseSerialReader


class ArduinoSerialReader:
    def __init__(self) -> None:
        super().__init__()
        self.arduino = serial.Serial(
            port="/dev/ttyACM0", baudrate=38400, timeout=1.0
        )

    async def read_serial(self) -> str:
        data = self.arduino.read_all()
        return data.decode("utf-8")
