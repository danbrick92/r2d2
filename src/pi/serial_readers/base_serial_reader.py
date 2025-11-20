from abc import ABC, abstractmethod


class BaseSerialReader(ABC):

    async def start(self) -> None:
        pass

    @abstractmethod
    async def read_serial(self) -> str:
        raise NotImplementedError("read_serial not implemented")
