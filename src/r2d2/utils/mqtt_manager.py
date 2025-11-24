from logging import Logger

import paho.mqtt.client as mqtt

from r2d2.utils.config import Config


class MQTTManager:
    def __init__(self, config: Config, logger: Logger):
        self.config = config
        self.logger = logger

    def get_mqtt_client(self) -> mqtt.Client:
        mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        return mqttc

    async def connect_mqtt_client(self, mqttc: mqtt.Client) -> None:
        mqttc.connect(
            host=self.config.mqtt_url,
            port=self.config.mqtt_port,
            keepalive=self.config.mqtt_keepalive,
        )
