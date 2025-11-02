#pragma once

//libs:
//standart/general use
#include <iostream>
#include <csignal>
#include <unistd.h>

//joystick
#include <SDL2/SDL.h>

//speedSensor
#include <thread>
#include <pigpio.h>

#include "../libs/include/I2c.hpp"
#include "exceptions.hpp"

//init
SDL_Joystick*	initCar();
void			initGpio();

//utils
void			signalHandler(int signum);
void			cleanExit();
double			mapAxisToAngle(double axisValue, double angleMin, double angleMax, double angleCenter);

//sensors
void			wheelRotationCalculation();

extern SDL_Joystick*    g_joystick;
extern int				g_pulses;

//index of the controller, if 0, the first, 
//and only the first, has permission to connect
#define CONTROLLER_0    0

//value of the axis in the controller
#define MAX_AXIS_VALUE	32767.0f

//Numbers of pulses from each rotation (how many holes the wheel has)
#define	PULSES_WHEEL	20

//angle to centralize the servo of the car and use it as the default angle
#define MID_ANGLE       60

#define A_BUTTON		0
#define B_BUTTON		1
#define X_BUTTON		3
#define Y_BUTTON		4
#define L1_BUTTON		6
#define R1_BUTTON		7
#define L2_BUTTON		8
#define R2_BUTTON		9
#define START_BUTTON	11
