import serial

# Define the serial port (replace '/dev/ttyUSB0' with your actual port)
serial_port = '/dev/ttyUSB0'

# Open the serial port
try:
    ser = serial.Serial(serial_port, baudrate=9600, timeout=1)
    print("Serial port opened successfully")
except serial.SerialException as e:
    print(f"Failed to open serial port: {e}")
    exit()

# Send data to the serial device
try:
    data = b'Hello, serial device!\n'
    ser.write(data)
    print(f"Data sent: {data.decode()}")
except Exception as e:
    print(f"Failed to send data: {e}")

# Close the serial port
ser.close()
print("Serial port closed")
