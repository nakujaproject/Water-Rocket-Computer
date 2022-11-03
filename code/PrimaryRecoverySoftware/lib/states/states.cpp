#include <states.h>
#include <defs.h>
#include <ejection.h>

// TODO: evaluate implication on time
const int MIN_APOGEE_COUNT = 20;

const int MIN_GROUND_COUNT = 20;

const int MIN_TAKE_OFF_COUNT = 20;

static int apogeeCounter = 0;
static int groundCounter = 0;
static int takeOffCounter = 0;


static float groundSum = 0;
static float meanCounter = 0;

// TODO: review this values
const float TAKE_OFF_AGL_DISPLACEMENT = 10;
const float DROGUE_DEPLOYMENT_BELOW_APOGEE_DISPLACEMENT = 0.2;
const float MAIN_DEPLOYMENT_BELOW_DROGUE_DISPLACEMENT = 0.2;
const int NUMBER_OF_READINGS_BEFORE_AVERAGE = 30;
const float MAX_GROUND_STATE_DEVIATION = 3;

static float max_altitude;
static float drogue_altitude;

// This checks that we have started ascent
// If we have a positive 10 metres displacement upwards
int checkInFlight(float altitude, float velocity)
{
    float displacement = altitude - base_altitude;
    
    if(displacement > TAKE_OFF_AGL_DISPLACEMENT){
    	takeOffCounter++;
    }

    if (takeOffCounter==MIN_TAKE_OFF_COUNT)
    {
        writeFlightEventToFlash(millis(), altitude, velocity, IN_FLIGHT_STATE);
        return IN_FLIGHT_STATE;
    }
    else
    {
        return PRE_FLIGHT_GROUND_STATE;
    }
}

// This checks that we have reached apogee and begun descent
// we check if previous altitude is greater than current altitude. To be certain, this has to be true at least 15 times
int checkForApogee(float velocity, float currentAltitude, float previousAltitude)
{
    if (currentAltitude < previousAltitude)
    {
        apogeeCounter++;
    }
    if (apogeeCounter == MIN_APOGEE_COUNT)
    {
        max_altitude = currentAltitude;
        writeFlightEventToFlash(millis(), currentAltitude, velocity, BALLISTIC_DESCENT_STATE);
        return BALLISTIC_DESCENT_STATE;
    }
    else
    {
        return IN_FLIGHT_STATE;
    }
}

// Deploys drogue parachute if we moved down x metres below apogee
int AttemptDeployDrogueChute(float altitude, float velocity)
{
    float displacement = max_altitude - altitude;
    if (displacement > DROGUE_DEPLOYMENT_BELOW_APOGEE_DISPLACEMENT)
    {
        drogue_altitude = altitude;
        int time_now = millis();

        // drogue ejection
        ejection(DROGUE_EJECTION_PIN);

        writeFlightEventToFlash(time_now, altitude, velocity, DROGUE_CHUTE_DESCENT_STATE);
        return DROGUE_CHUTE_DESCENT_STATE;
    }
    else
    {
        return BALLISTIC_DESCENT_STATE;
    }
}

// Deploys Main parachute if we moved down y metres below drogue
int AttemptDeployMainChute(float altitude, float velocity)
{
    float displacement = drogue_altitude - altitude;
    if (displacement > MAIN_DEPLOYMENT_BELOW_DROGUE_DISPLACEMENT)
    {
        writeFlightEventToFlash(millis(), altitude, velocity, MAIN_CHUTE_DESCENT_STATE);

        // main ejection
        ejection(MAIN_EJECTION_PIN);

        return MAIN_CHUTE_DESCENT_STATE;
    }
    else
    {
        return DROGUE_CHUTE_DESCENT_STATE;
    }
}

// This checks that we have reached the ground, detects landing of the rocket
// here we compare the current altitude reading with the average of the last 30 altitude readings. If their difference is less than or equal to 2m for 6 consecutive times, then we have landed on the ground
int checkGround(float currentAltitude, float velocity)
{
    groundSum += currentAltitude;
    meanCounter++;

    if (meanCounter == NUMBER_OF_READINGS_BEFORE_AVERAGE)
    {
        float mean = groundSum / meanCounter;
        if (abs(mean - currentAltitude) <= MAX_GROUND_STATE_DEVIATION)
        {
            groundCounter++;
        }
        else
        {
            groundCounter = 0;
        }
        // prepare for a next set of 30 readings
        groundSum = 0;
        meanCounter = 0;
    }

    if (groundCounter == MIN_GROUND_COUNT)
    {
        writeFlightEventToFlash(millis(), currentAltitude, velocity, POST_FLIGHT_GROUND_STATE);

        return POST_FLIGHT_GROUND_STATE;
    }
    else
    {
        return MAIN_CHUTE_DESCENT_STATE;
    }
}

// Updates the state-machine state
// We check if rocket has launched to move from PRE_FLIGHT_GROUND_STATE to IN_FLIGHT_STATE
// We check if we have reached apogee to move to BALLISTIC_DESCENT_STATE
// We deploy drogue parachute to move to DROGUE_CHUTE_DESCENT_STATE
// We deploy main parachute to move to MAIN_CHUTE_DESCENT_STATE
// We check if we have reached the ground to move to POST_FLIGHT_GROUND_STATE
int checkState(float currentAltitude, float previousAltitude, float velocity, float acceleration, int state)
{
    switch (state)
    {
    case PRE_FLIGHT_GROUND_STATE:
        return checkInFlight(currentAltitude, velocity);
    case IN_FLIGHT_STATE:
        return checkForApogee(velocity, currentAltitude, previousAltitude);
    case BALLISTIC_DESCENT_STATE:
        return AttemptDeployDrogueChute(currentAltitude, velocity);
    case DROGUE_CHUTE_DESCENT_STATE:
        return AttemptDeployMainChute(currentAltitude, velocity);
    case MAIN_CHUTE_DESCENT_STATE:
        return checkGround(currentAltitude, velocity);
    case POST_FLIGHT_GROUND_STATE:
        return POST_FLIGHT_GROUND_STATE;
    default:
        return checkInFlight(currentAltitude, velocity);
    }
}
