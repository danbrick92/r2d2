from pydantic import BaseModel
from states.serial_initializer import SerialInitializer


class SerialInitializerRoute(BaseModel):
    state: type[SerialInitializer]
    topic: str
