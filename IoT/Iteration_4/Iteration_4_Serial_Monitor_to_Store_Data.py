import asyncio
from bleak import BleakScanner, BleakClient
import struct
import time
import csv

def choose_arduino(): 
    while True:
        arduino_we_are_using = input("Which Arduino are we using? fathu (1), ivorine (2)? or aish (3): ")
        if arduino_we_are_using == "fathu" or arduino_we_are_using == "1":
            arduino_we_are_using = "c4:ca:75:08:5b:c6"
            print("Fathu's Arduino Nano 33 BLE is chosen. Searching for the device...")
            break
        elif arduino_we_are_using == "ivorine" or arduino_we_are_using == "2":
            arduino_we_are_using = "61:D3:7A:2A:F3:6B"
            print("Ivorine's Arduino Nano 33 BLE is chosen. Searching for the device...")
            break
        elif arduino_we_are_using == "aish" or arduino_we_are_using == "3":
            arduino_we_are_using = "19:63:f5:f4:82:f3"
            print("Aish's Arduino Nano 33 BLE is chosen. Searching for the device...")
            break
        else:
            print("Invalid input. Please try again.")
            continue
    return arduino_we_are_using

# Replace this with your Arduino Nano 33 BLE's address
fathu_arduino = "c4:ca:75:08:5b:c6"
ivorine_arduino = "61:D3:7A:2A:F3:6B"
aish_arduino = "19:63:f5:f4:82:f3"

# UUID of the characteristic you want to read from
gesture_char_uuid = "19b10001-e8f2-537e-4f6c-d104768a1214"

# UUIDs for Accelerometer, Gyroscope, and Magnetometer
accXChar_uuid = "2a5b0001-1234-5678-1234-56789abcdef0"
accYChar_uuid = "2a5b0002-1234-5678-1234-56789abcdef0"
accZChar_uuid = "2a5b0003-1234-5678-1234-56789abcdef0"

gyroXChar_uuid = "2a5b0004-1234-5678-1234-56789abcdef0"
gyroYChar_uuid = "2a5b0005-1234-5678-1234-56789abcdef0"
gyroZChar_uuid = "2a5b0006-1234-5678-1234-56789abcdef0"

magXChar_uuid = "2a5b0007-1234-5678-1234-56789abcdef0"
magYChar_uuid = "2a5b0008-1234-5678-1234-56789abcdef0"
magZChar_uuid = "2a5b0009-1234-5678-1234-56789abcdef0"

# Function to discover BLE devices
async def scan_devices():
    devices = await BleakScanner.discover(timeout=2.0)
    # for device in devices:
    #     print(f"Device found: {device.name}, Address: {device.address}")
    return devices

# Function to read sensor data from the BLE device
async def read_sensor_data(device_address):
    async with BleakClient(device_address) as client:
        if client.is_connected:
            # print(f"Connected to {device_address}")
            print(f"Connected to the Arduino")
            print(f"Telling Arduino to start sending data...")
            data_to_write = struct.pack('i', 4)
            await client.write_gatt_char(gesture_char_uuid, data_to_write)
            gesture_data = await client.read_gatt_char(gesture_char_uuid)
            header = [
                ["acc_x (m/s^2)","acc_y (m/s^2)","acc_z (m/s^2)","gyro_x (degree/seconds)","gyro_y (degree/seconds)","gyro_z (degree/seconds)","mag_x (uT)","mag_y (uT)","mag_z (uT)"]
            ]
                
            # Open the file for writing with quoting enabled
            with open('9DOF_readings.csv', mode='w', newline='') as file:
                writer = csv.writer(file)

                # Write the data rows to the file
                writer.writerows(header)

            # print(f"Gesture data is now {gesture_data}...")
            print("Arduino is starting to send data...")
            while client.is_connected:

                # Read each characteristic and convert to float
                accXChar_data = await client.read_gatt_char(accXChar_uuid)
                accYChar_data = await client.read_gatt_char(accYChar_uuid)
                accZChar_data = await client.read_gatt_char(accZChar_uuid)
                
                gyroXChar_data = await client.read_gatt_char(gyroXChar_uuid)
                gyroYChar_data = await client.read_gatt_char(gyroYChar_uuid)
                gyroZChar_data = await client.read_gatt_char(gyroZChar_uuid)
                
                magXChar_data = await client.read_gatt_char(magXChar_uuid)
                magYChar_data = await client.read_gatt_char(magYChar_uuid)
                magZChar_data = await client.read_gatt_char(magZChar_uuid)
                
                # Convert byte data to float using struct.unpack
                accXChar = struct.unpack('f', accXChar_data)[0]
                accYChar = struct.unpack('f', accYChar_data)[0]
                accZChar = struct.unpack('f', accZChar_data)[0]
                
                gyroXChar = struct.unpack('f', gyroXChar_data)[0]
                gyroYChar = struct.unpack('f', gyroYChar_data)[0]
                gyroZChar = struct.unpack('f', gyroZChar_data)[0]
                
                magXChar = struct.unpack('f', magXChar_data)[0]
                magYChar = struct.unpack('f', magYChar_data)[0]
                magZChar = struct.unpack('f', magZChar_data)[0]
                
                data = [
                    [accXChar,accYChar,accZChar,
                    gyroXChar,gyroYChar,gyroZChar,
                    magXChar,magYChar,magZChar]
                ]
                
                # Open the file for writing with quoting enabled
                with open('9DOF_readings.csv', mode='a', newline='') as file:
                    writer = csv.writer(file)

                    # Write the data rows to the file
                    writer.writerows(data)

                print(f"{accXChar},{accYChar},{accZChar},"
                  f"{gyroXChar},{gyroYChar},{gyroZChar},"
                  f"{magXChar},{magYChar},{magZChar}")
                
        else:
            print(f"Failed to connect to {device_address}")

# Main function to scan for devices, then read data from a specific one
async def main():
    arduino_we_are_using = choose_arduino()
    devices = await scan_devices()  # Discover available devices
    if devices:
        # Choose the correct device address (replace with your device's address)
        device_address = arduino_we_are_using

        # Read sensor data
        await read_sensor_data(device_address)

# Run the main function
asyncio.run(main())