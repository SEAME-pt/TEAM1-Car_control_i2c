#include "../../include/sdl.h"

// global variable to count pulses on the wheel
std::atomic<int>	g_pulses{0};

void	wheelRotationCalculation() {
	
	while (true) {
		// sleep for measurement period
		gpioDelay(5000000); // 5 seconds in microseconds

		// read and reset pulse count atomically
		int pulses = g_pulses.exchange(0);
		
		// calculate RPM: (pulses in 5s / PULSES_PER_REV) * (60s / 5s)
		double rpm = (pulses / 5.0) * 60.0 / PULSES_WHEEL;

		std::cout << pulses << " pulses, rotating at "
			<< rpm << " RPM" << std::endl;
	}
}

void	pulse_callback(int gpio, int level, uint32_t tick) {

	(void)gpio;
    (void)tick;
	if (level == 0) // FALLING edge
		g_pulses++;
}
