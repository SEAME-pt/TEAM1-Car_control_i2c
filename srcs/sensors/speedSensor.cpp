#include "../../include/sdl.h"

// global variable to count pulses on the wheel
int	g_pulses = 0;

void	wheelRotationCalculation() {
	
	g_pulses = 0;
	double rpm = (g_pulses / 5.0) * 60.0 / PULSES_WHEEL;

	std::cout << g_pulses << " signals and is rotation at "
		<< rpm << " per minute." << std::endl;
}

void	pulse_callback(int gpio, int level, uint32_t tick) {

	if (!level) // FALLING edge
		g_pulses++;
}
