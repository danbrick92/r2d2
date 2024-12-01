import serial
import keyboard


BAUD_RATE = 9600
SERIAL_PORT = "/dev/ttyACM0"


def get_input(direction: int, magnitude: int) -> str:
    # Wait for a single keypress
    print("Use WASD to move, Q to decelerate, E to accelerate: ")
    char = keyboard.read_event(suppress=True).name

    # Map to correct action
    if char.lower() == "w":
        direction = 90
    elif char.lower() == "d":
        direction = 0
    elif char.lower() == "s":
        direction = 270
    elif char.lower() == "a":
        direction = 180
    elif char.lower() == "q":
        magnitude -= 10
    elif char.lower() == "e":
        magnitude += 10

    # Regulate direction and magnitude
    direction = direction % 360
    magnitude = min(max(magnitude, -255), 255)
    return direction, magnitude


def read_serial(arduino: serial.Serial) -> None:
    data = arduino.readline()
    print(data)


def write_movement(arduino: serial.Serial, magnitude: int, direction: int) -> None:
    data = f"(motors,set,magntitude={magnitude},direction={direction})"
    arduino.write(bytes(data, 'utf-8'))


def main():
    arduino = serial.Serial(port=SERIAL_PORT, baudrate=BAUD_RATE, timeout=.1) 
    direction, magnitude = 0, 0
    print("Beginning interface with Arduino")
    while True:
        read_serial(arduino)
        direction, magnitude = get_input(direction, magnitude)


if __name__ == "__main__":
    main()
