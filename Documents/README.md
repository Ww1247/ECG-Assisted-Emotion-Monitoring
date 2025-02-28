# Hardware Construction Manual

This section documents the process of building the hardware, as well as problems and solutions that may be encountered during the build.

## Sensors List

| Item | Sensor Name                      | Model    | Description                                                    | Amount (pcs)| Datasheet   |
|:----:|----------------------------------|:--------:|----------------------------------------------------------------|:-----------:|:-----------:|
|  1   | WebCam                           | WebCam   | Capture facial images and photos                               | 1           |             |
|  2   | ECG Sensor                       | MAX30102 | ECG, pulse and blood oxygen data were collected                | 1           |             |
|  3   | Humidity and Temperature Sensors | AHT20    | Humidity and temperature changes in the fingers were collected | 1           |             |

## Connect Sensors to Raspberry Pi

<p align="center">
  <img src="./pictures/sensors_wire_connection.png" width="500"/>
</p>

The circuit diagram has been created by *Frizing*, you can follow the instructions above to connect the circuit and make sure all sensors are connected to the Raspberry Pi through the correct wiring, if you have more than one sensor, just extend the connections on the I2C bus (SDA and SCL). The circuit diagram files drawn by *Frizing* have been uploaded to the project repository named as `sensors_connection_instruction.fzz` and can be viewed using the *Frizing* software.

### Raspberry Pi

- **GND** (Ground): The black wire is connected to the **GND** pin on the Raspberry Pi.
- **VIN** (Power Input): The red wire is connected to either the **5V** or **3.3V** pin on the Raspberry Pi. **Note**: This depends on the voltage requirement of the sensor (typically 5V or 3.3V).
- **SDA** (Data Line): The orange wire is connected to **GPIO 2** (SDA) on the Raspberry Pi.
- **SCL** (Clock Line): The blue wire is connected to **GPIO 3** (SCL) on the Raspberry Pi.

### Sensors (Multiple I2C Sensors)

- The **GND** pin of each sensor is connected to the **GND** pin on the Raspberry Pi (black wire).
- The **VIN** pin of each sensor is connected to either the **5V** or **3.3V** pin on the Raspberry Pi (red wire), ensuring voltage consistency.
- The **SDA** pin of each sensor is connected to the **SDA** bus, i.e., **GPIO 2** on the Raspberry Pi (orange wire).
- The **SCL** pin of each sensor is connected to the **SCL** bus, i.e., **GPIO 3** on the Raspberry Pi (blue wire).

> [!NOTE]  
> 1. **I2C Address Conflict**: If connecting multiple I2C devices, make sure their addresses do not conflict. If there is a conflict, it can be resolved by setting the sensor addresses (via jumpers or software configuration).
> 2. **Voltage Matching**: Ensure the sensor power voltage matches the voltage provided by the Raspberry Pi. Sensors typically support either 3.3V or 5V, depending on the sensor model.
> 3. **I2C Pins**: Make sure to use **GPIO 2** (SDA) and **GPIO 3** (SCL) on the Raspberry Pi for data transfer lines.

## Sensor Development Guide

This project uses the `pigpio` library to provide efficient GPIO control and interaction with external sensors on the Raspberry Pi. `pigpio` is a C language library running on the Raspberry Pi that offers powerful GPIO control features, including digital input, output, PWM, I2C, SPI, and serial communication. The reason for using the `pigpio` library in this project is that it provides precise timing control for sensors, supports multithreading, and can run stably in a concurrent environment, making it suitable for complex sensor data acquisition and processing tasks.

If `pigpio` is not yet installed, you can install it with the following command:

```bash
sudo apt install pigpio
```

To verify if `pigpio` is correctly installed, run the following command:

```bash
pigpiod -v
```

<br>To correctly link the `pigpio` library in CMake, configure the `CMakeLists.txt` file as follows:

```bash
# Set the path for the pigpio library
find_library(PIGPIO_LIB pigpio)

# Link the pigpio library
target_link_libraries(Project_Name ${PIGPIO_LIB})
```

<br>Here is a simple example of how to use the `pigpio` library to call I2C:

```C++
#include <pigpio.h>

// Initialize the pigpio library
if (gpioInitialise() < 0) {
    std::cerr << "pigpio initialization failed!" << std::endl;
    return -1; // Initialization failed, exit the program
}

int i2cHandle = i2cOpen(1, 0x40, 0);  // Open I2C bus 1, device address 0x40
if (i2cHandle < 0) {
    std::cerr << "Failed to open I2C bus!" << std::endl;
    return -1;
}

char data[] = {0x01, 0xFF};  // Send data, write 0xFF to register 0x01
i2cWrite(i2cHandle, data, sizeof(data));

char buffer[10];
int readBytes = i2cRead(i2cHandle, buffer, sizeof(buffer)); // Read data
if (readBytes >= 0) {
    std::cout << "I2C read data: ";
    for (int i = 0; i < readBytes; i++) {
        std::cout << (int)buffer[i] << " ";
    }
    std::cout << std::endl;
}

gpioTerminate();  // Terminate the pigpio library
```

### Pulse Sensor

### Humidity and Temperature Sensor

### Camera
