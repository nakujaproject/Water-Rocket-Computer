#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>
#include <ESP32Servo.h>

#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif // DEBUG

#define PYRO_EJECTION false
#define READ_FLASH true

#define BAUD_RATE 115200

extern float base_altitude;
extern int state;

// TODO ejection pins for water rocket, update for pyro ejection
const uint8_t MAIN_EJECTION_PIN = 4;
const uint8_t DROGUE_EJECTION_PIN = 23;

// state machine states
const int PRE_FLIGHT_GROUND_STATE = 0;
const int IN_FLIGHT_STATE = 1;
const int BALLISTIC_DESCENT_STATE = 2;
const int DROGUE_CHUTE_DESCENT_STATE = 3;
const int MAIN_CHUTE_DESCENT_STATE = 4;
const int POST_FLIGHT_GROUND_STATE = 5;

extern Servo *drogueServo;
extern Servo *mainServo;

#endif // DEFS_H