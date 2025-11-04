#include "../../include/sdl.h"

// global variable to count pulses on the wheel
std::atomic<int>	g_pulses{0};

void	wheelRotationCalculation() {
	
	std::cout << "Speed sensor thread started\n";
	while (true) {
		// sleep for measurement period
		gpioDelay(5000000); // 5 seconds in microseconds

		// read and reset pulse count atomically
		int pulses = g_pulses.exchange(0);
		
		if (pulses > 0) {
            // calculate rotations per minute
            double rpm = (pulses / (double)PULSES_WHEEL) * 12.0; // (60s / 5s) = 12
            std::cout << "Pulses: " << pulses << ", RPM: " << rpm << std::endl;
        }
	}
}

void	pulse_callback(int gpio, int level, uint32_t tick) {

	(void)gpio;
    (void)tick;
	if (level == 0) // FALLING edge
		g_pulses.fetch_add(1, std::memory_order_relaxed);
}
