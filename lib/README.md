# TEAM1_I2C

### What is I²C?

The **Inter-Integrated Circuit (I²C)** protocol is a widely used communication standard in electronics for connecting multiple low-speed peripherals to a microcontroller or processor using only two wires:
- **SDA** (data line)  
- **SCL** (clock line)

---

### How to Use on Raspberry Pi

To use the I²C protocol on a Raspberry Pi, we need to identify the I²C bus interface, typically something like `/dev/i2c-1`.  
Inside this interface, there’s a table of device addresses that can be scanned using the command:

```bash
i2cdetect -y 1
```

You’ll see an output similar to this:

```
     0 1 2 3 4 5 6 7 8 9 a b c d e f
00:          -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- 38 -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

---

### I²C Controllers

There are two types of I²C controllers:

- **Master:** Controls communication and initiates data transfers.  
- **Slave:** Responds to master requests.  

Although it is *technically possible* to have multiple masters, this is rarely used in practice.

---

### Using I²C in C

In C, we can use the Linux I²C library:

```c
#include <linux/i2c-dev.h>
```

#### Basic functions:

```c
open()     → open I²C bus
ioctl()    → configure address or special parameters
read()/write() → exchange data
```

The main function for configuration is:

```c
ioctl(fd, functionality, address)
```

This function allows setting different modes such as:

| Macro | Description |
|--------|-------------|
| **I2C_SLAVE** | Selects the slave address (7 or 10 bits) |
| **I2C_SLAVE_FORCE** | Forces communication even if the bus is busy |
| **I2C_TENBIT** | Defines a 10-bit slave address |
| **I2C_FUNCS** | Queries the driver for supported functionalities |
| **I2C_RDWR** | Performs combined read/write operation (`i2c_rdwr_ioctl_data`) |

---

### PCA9685

The **PCA9685** is a PWM (Pulse Width Modulation) controller that can be used to control servos and DC motors via the I²C communication protocol.

This library implements I²C communication to control **two DC motors** and **one servo motor**.

---

### Example: Motor Control

```c++
int main() {

	I2c::All_init();
    I2c::motor(0, 100, 1);
    sleep(5);
    I2c::stop_motors();
}
```

---

### Motor Control Function

```c++
void I2c::motor(int mot, int speed, bool dir);
```

#### Parameters:

| Parameter | Description |
|------------|-------------|
| **mot** | Selects which motor to activate:<br>• `1` → first motor<br>• `2` → second motor<br>• `0` → both motors |
| **speed** | Motor speed (0–100) |
| **dir** | Rotation direction:<br>• `0` → one direction<br>• `1` → opposite direction |

To stop the motors, either set the speed to **0** or use:

```c++
I2c::stop_motors();
```
In case of any doubt use the  I2c::stop_motors(); for reset the motors. 


# INA219

The **INA219** is an I²C chip responsible for providing battery information such as **voltage**, **current**, and **power**.

## Class Structure

The `I2c_INA219` class is included within the `I2c` class, so initialization can be done simply with:

```cpp

	I2c::All_init();
```

The sensor readings are stored in the following `I2c` class variables:

```cpp
I2c::_Voltage;
I2c::_Current;
I2c::_Power;
```

## Updating Values

To manually update the sensor readings, use:

```cpp
I2c_INA219::update_values();
```

## Automatic Update on Print

When using the print function, the values are automatically updated before being displayed:

```cpp
I2c_INA219::print();
```

---

### Summary

| Function                      | Description                                            |
| ----------------------------- | ------------------------------------------------------ |
| `I2c_INA219::update_values()` | Updates the voltage, current, and power readings.      |
| `I2c_INA219::print()`         | Automatically updates and prints the current readings. |
| `I2c::stop_motors()`          | Example of how to initialize the `I2c` class.          |

---

### Notes

* Make sure your I²C bus is properly initialized before calling any INA219 functions.
* The `I2c_INA219` class handles sensor communication and stores data internally for easy access.

---


## CMake

### Basic Usage

To build the project using **CMake**, follow these steps:

1. Create a folder named `build` (if it does not already exist).  
   If it exists, remove all existing files inside it to ensure a clean build.

2. Inside the `build` directory, run the following command to generate the Makefiles using `CMakeLists.txt`:

   ```bash
   cmake ..

