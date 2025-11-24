from abc import ABC, abstractmethod

from r2d2.utils.context import Context, ContextMixin


class BaseNode(ABC, ContextMixin):
    def __init__(self, context: Context) -> None:
        self.set_vars(context)

    @abstractmethod
    async def init(self) -> None:
        raise NotImplementedError("init not implemented")

    @abstractmethod
    async def run(self) -> None:
        raise NotImplementedError("run not implemented")

    @abstractmethod
    async def cleanup(self) -> None:
        raise NotImplementedError("cleanup not implemented")
