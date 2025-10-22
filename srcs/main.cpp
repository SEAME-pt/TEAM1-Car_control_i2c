#include "../lib/include/I2c.hpp"

int main() {

    I2c::All_init();
    I2c::motor(0,50,1);
    sleep(3);
    I2c::stop_motors();
    I2c::set_servo_angle(90);
    sleep(3);
    I2c::motor(0,50,1);
    return 0;
}
