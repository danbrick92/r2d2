from typing import Any
import paho.mqtt.client as mqtt


class MQTTClient:
    def __init__(self):
        self.mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

    def connect(
        self, url: str = "localhost", port: int = 1883, keepalive: int = 60
    ) -> None:
        self.mqttc.connect(url, port, keepalive)

    def publish(self, topic: str, payload: Any) -> None:
        self.mqttc.publish(topic=topic, payload=payload)
