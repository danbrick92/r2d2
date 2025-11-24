from logging import Logger
from typing import Optional

from r2d2.utils.config import Config
from r2d2.utils.logging import LoggerMixin, setup_logging
from r2d2.utils.mqtt_manager import MQTTManager


class Context:
    def __init__(
        self,
        config: Optional[Config] = None,
        logger: Optional[Logger] = None,
        mqtt_manager: Optional[MQTTManager] = None,
    ) -> None:
        if config is None:
            config = Config()
        self.config = config

        if logger is None:
            logger = setup_logging()
        self.logger = logger

        if mqtt_manager is None:
            mqtt_manager = MQTTManager(config, logger)
        self.mqtt_manager = mqtt_manager


class ContextMixin(LoggerMixin):
    def set_vars(self, context: Context, set_mqtt_client: bool = True) -> None:
        self.context = context
        self.config = context.config
        self.logger = context.logger
        if set_mqtt_client:
            self.mqtt_client = context.mqtt_manager.get_mqtt_client()
