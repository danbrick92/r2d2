from abc import ABC, abstractmethod
from logging import Logger

from r2d2.utils.logging import LoggerMixin


class BaseSerialReader(ABC, LoggerMixin):
    def __init__(self, logger: Logger) -> None:
        self.logger = logger

    async def start(self) -> None:
        pass

    def whoami(self) -> str:
        return self.__class__.__name__

    @abstractmethod
    async def read_serial(self) -> str:
        raise NotImplementedError("read_serial not implemented")

    async def catchup(self) -> None:
        raise NotImplementedError("catchup used but not implemented")
