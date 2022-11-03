#include <servo.h>

Servo *drogueServo = new Servo;
Servo *mainServo = new Servo;

// set up ejection PMW pins
void setUpEjectionServos()
{
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    drogueServo->setPeriodHertz(50);          // standard 50 hz servo
    drogueServo->attach(DROGUE_EJECTION_PIN); // attaches the servo on drogue pin to the servo object
    drogueServo->write(180);

    mainServo->setPeriodHertz(50);        // standard 50 hz servo
    mainServo->attach(MAIN_EJECTION_PIN); // attaches the servo on main pin to the servo object
    mainServo->write(180);

    delay(2000);
}