from abc import ABC, abstractmethod


class SerialInitializer(ABC):
    @staticmethod
    @abstractmethod
    def initialize_from_serial(serial_line: str):
        raise NotImplementedError("initialize_from_serial not implemented")
