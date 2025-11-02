#include "../../include/sdl.h"

// global variable to count pulses on the wheel
int	g_pulses = 0;

void	wheelRotationCalculation() {
	
	g_pulses = 0;
    double rpm = (g_pulses / 5.0) * 60.0 / PULSES_WHEEL;
    printf("Pulsos=%d, RPM≈%.1f\n", g_pulses, rpm);
}