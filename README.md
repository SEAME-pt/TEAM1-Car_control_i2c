# TEAM1 - Velocity Era

Repository created to consolidate the most up to date software stack for our autonomous vehicle, affectionately called Vera.

## Overview

This program requires a controller to register joystick input and control a car via I²C communication, while a speed sensor measures the car’s velocity and outputs the data on the display with QT framework.

This platform integrates:
- Rapsberry Pi 5;
- Microcontroller (STM32);
- Speed sensor;

---

## File Structure

```
srcs/
├── main.cpp                    → Main logic & control loop
├── exit/
│   ├── exitCleanups.cpp       → Resource cleanup functions (cleanExit, exitCar)
│   └── signals.cpp            → Signal handler (SIGINT/Ctrl+C)
├── init/
│   ├── init_car_i2c.cpp       → SDL joystick + I2C initialization
│   └── init_gpio.cpp          → GPIO + wheel sensor setup
├── sensors/
│   └── speedSensor.cpp        → Wheel rotation speed calculation (threading)
└── utils/
    └── math_utils.cpp         → Axis-to-angle mapping utilities

include/
├── sdl.h                      → Definitions, constants & function declarations
└── exceptions.hpp             → Custom exception classes (InitException)

libs/                          → I2C library (submodule/external)
├── CMakeLists.txt            → I2C library build configuration
├── include/
│   ├── I2c.hpp               → I2C wrapper interface
│   ├── I2c_PcA9685.hpp       → PCA9685 PWM driver (servo/motor control)
│   └── I2c_INA219.hpp        → INA219 current/voltage sensor
├── srcs/
│   ├── I2c.cpp               → I2C wrapper implementation
│   ├── I2c_PcA9685.cpp       → PCA9685 driver implementation
│   └── I2c_INA219.cpp        → INA219 sensor implementation
```

## Build System

---

## Build
```bash
mkdir build && cd build
cmake ..
make
sudo ./car
```

# 3D Car Design

![3D_Car](https://github.com/user-attachments/assets/8f43435f-64be-477a-a1f1-4633c5882d7c)

---

## Team members

- Jose Meneses
- Afonso
- Rafael
- Joao

For a more detailed overview, please refer to the next readme.md file.
