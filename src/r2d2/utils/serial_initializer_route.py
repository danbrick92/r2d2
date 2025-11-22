from pydantic import BaseModel

from r2d2.states.serial_initializer import SerialInitializer


class SerialInitializerRoute(BaseModel):
    state: type[SerialInitializer]
    topic: str
