from abc import ABC, abstractmethod

from r2d2.utils.context import Context, ContextMixin


class BaseSerialReader(ABC, ContextMixin):
    def __init__(self, context: Context) -> None:
        self.set_vars(context, set_mqtt_client=False)

    async def start(self) -> None:
        pass

    def whoami(self) -> str:
        return self.__class__.__name__

    @abstractmethod
    async def read_serial(self) -> str:
        raise NotImplementedError("read_serial not implemented")

    async def catchup(self) -> None:
        raise NotImplementedError("catchup used but not implemented")
