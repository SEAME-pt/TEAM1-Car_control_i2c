# 🕹️ SDL Joystick Car Controller

## Overview
This program uses **SDL2** to read joystick input and control a car via **I²C** communication.  
It translates joystick axes into **servo steering** and **motor throttle** values, sending real-time commands to an I²C motor controller.

---

## File Structure

```
srcs/
├─ main.cpp → Main logic & control loop
├─ initCar.cpp → Joystick + hardware initialization
include/
├─ sdl.h → Definitions & constants
└─ exceptions.hpp → SDL initialization exception
```

---

## Build
```bash
mkdir build && cd build
cmake ..
make
sudo ./car
```

 Main Components
🛑 signalHandler(int signum)

Handles Ctrl+C interrupts — stops all motors, quits SDL, and exits safely.

🔄 mapAxisToAngle(double axisValue, double angleMin, double angleMax, double angleCenter)

Maps a joystick axis value in [-1, 1] to an angle between angleMin and angleMax around a center value.

🎮 initCar()

Initializes SDL and I²C, opens the first joystick, sets the servo to its neutral position (MID_ANGLE), and returns the joystick pointer.
Throws GenException::InitException if initialization fails.

🚀 main()

Registers signal handler

Initializes joystick and I²C

Reads joystick axes continuously

Axis 2 → Steering angle [0–120°]

Axis 1 → Throttle [-100–100]

Sends I²C motor/servo commands:

Positive throttle → backward

Negative throttle → forward

Zero → stop motors

Handles Start button to exit cleanly

Loop updates every 15 ms

| Constant                                       | Description               |
| ---------------------------------------------- | ------------------------- |
| `CONTROLLER_0`                                 | First joystick            |
| `MID_ANGLE`                                    | Neutral servo angle (60°) |
| `START_BUTTON`                                 | Exits program             |
| `A_BUTTON`, `B_BUTTON`, `X_BUTTON`, `Y_BUTTON` | Reserved buttons          |


⚠️ Exceptions

GenException::InitException
Thrown when SDL or joystick initialization fails.
Returns SDL’s internal error message via SDL_GetError().

🧼 Exit Procedure

When exiting (Start button or Ctrl+C):

Stops all motors via I2c::stop_all()

Closes joystick and quits SDL

Exits cleanly

💡 Possible Improvements

Add joystick dead zone filtering

Smooth throttle transitions

Add debug/logging for axis values

🧱 Example Run

Joystick detected: Logitech Gamepad F310
Axis2 = -0.45 → Steering = 33°
Axis1 =  0.70 → Throttle = 70 (Backward)
...
Button start pressed. Exiting...

Author:

Developed for embedded car control using SDL2 + I²C.
Fully compatible with Linux environments.
