from abc import ABC, abstractmethod
from logging import Logger

from r2d2.utils.logging import LoggerMixin
from r2d2.utils.mqtt import MQTTClient
from r2d2.config.config import Config


class BaseNode(ABC, LoggerMixin):
    def __init__(self, mqtt_client: MQTTClient, logger: Logger, config: Config) -> None:
        self.mqtt_client = mqtt_client
        self.logger = logger
        self.config = config

    @abstractmethod
    async def init(self) -> None:
        raise NotImplementedError("init not implemented")

    @abstractmethod
    async def run(self) -> None:
        raise NotImplementedError("run not implemented")

    @abstractmethod
    async def cleanup(self) -> None:
        raise NotImplementedError("cleanup not implemented")
