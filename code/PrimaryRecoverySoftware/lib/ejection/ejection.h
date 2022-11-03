#ifndef EJECTION_H
#define EJECTION_H
#include <defs.h>

void DrogueEjectionTimerCallback(TimerHandle_t ejectionTimerHandle);
void MainEjectionTimerCallback(TimerHandle_t ejectionTimerHandle);
void ejection(uint8_t ejectionPin);
void servoEjection(uint8_t ejectionPin);
void pyroEjection(uint8_t ejectionPin);
void setPyroEjectionPinModes();

#endif // EJECTION_H