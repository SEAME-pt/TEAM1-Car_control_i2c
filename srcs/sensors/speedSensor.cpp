#include "../../include/sdl.h"

// global variable to count pulses on the wheel
std::atomic<int>	g_pulses{0};

void	wheelRotationCalculation() {
	
	std::cout << "Speed sensor thread started\n";
	while (g_running) {
		// sleep 1 second for measurement period
		gpioDelay(1000000);

		// read and reset pulse count atomically
		int pulses = g_pulses.exchange(0);
		
		if (pulses > 0) {
            double rpm = (pulses / (double)PULSES_WHEEL) * 12.0; // (60s / 5s) = 12
            std::cout << "Pulses: " << pulses << ", RPM: " << rpm << std::endl;
        }
	}
}

void	pulse_callback(int gpio, int level, uint32_t tick) {

	(void)gpio;
    (void)tick;

	if (level == 0)
		g_pulses.fetch_add(1, std::memory_order_relaxed);
}
