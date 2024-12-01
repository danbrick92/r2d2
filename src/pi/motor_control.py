import serial
import getch


BAUD_RATE = 9600
SERIAL_PORT = "/dev/ttyACM0"


def get_input(magnitude: int, direction: int) -> str:
    # Wait for a single keypress
    print("Use WASD to move, Q to decelerate, E to accelerate: ")
    char = getch.getch() 

    # Map to correct action
    if char.lower() == "w":
        direction = 0
    elif char.lower() == "d":
        direction = 90
    elif char.lower() == "s":
        direction = 180
    elif char.lower() == "a":
        direction = 270
    elif char.lower() == "q":
        magnitude -= 10
    elif char.lower() == "e":
        magnitude += 10

    # Regulate direction and magnitude
    direction = direction % 360
    magnitude = min(max(magnitude, -255), 255)
    return magnitude, direction


def read_serial(arduino: serial.Serial) -> None:
    data = arduino.readline()
    print(data)


def write_movement(arduino: serial.Serial, magnitude: int, direction: int) -> None:
    data = f"(motors,set,magnitude={magnitude},direction={direction})"
    arduino.write(bytes(data, 'utf-8'))


def main():
    arduino = serial.Serial(port=SERIAL_PORT, baudrate=BAUD_RATE, timeout=1.0) 
    magnitude, direction = 0, 0
    print("Beginning interface with Arduino")
    while True:
        read_serial(arduino)
        magnitude, direction = get_input(magnitude, direction)
        write_movement(arduino, magnitude, direction)
    print("Exited")


if __name__ == "__main__":
    main()
