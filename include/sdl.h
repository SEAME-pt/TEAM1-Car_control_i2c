#pragma once

#include "../libs/TEAM1_I2C/include/I2c_PcA9685.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>

#include "exceptions.hpp"

#define CONTROLLER_0 0
#define MID_ANGLE 60

#define START_BUTTON	11
#define X_BUTTON		3
#define A_BUTTON		0
#define B_BUTTON		1
#define Y_BUTTON		4
#define R1_BUTTON		7
#define R2_BUTTON		9
#define L1_BUTTON		6
#define L2_BUTTON		8
