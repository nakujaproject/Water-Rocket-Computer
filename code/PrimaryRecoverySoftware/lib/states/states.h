#ifndef STATES_H
#define STATES_H

#include <defs.h>
#include <flash_memory.h>

int checkState(float currentAltitude, float previousAltitude, float velocity, float acceleration, int state);
int checkGround(float currentAltitude, float velocity);
int AttemptDeployMainChute(float altitude, float velocity);
int AttemptDeployDrogueChute(float altitude, float velocity);
int checkForApogee(float velocity, float currentAltitude, float previousAltitude);
int checkInFlight(float altitude, float velocity);

#endif // STATES_H