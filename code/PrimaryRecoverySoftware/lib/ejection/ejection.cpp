#include <ejection.h>
#include <flash_memory.h>

extern TaskHandle_t DrogueEjectTaskHandle;
extern TaskHandle_t MainEjectTaskHandle;

TimerHandle_t ejectionTimerHandle = NULL;

// TODO: carry out tests to know how much time
const int EJECTION_PIN_ON_TIME_MS = 1000;

TimerHandle_t mainEjectionTimerHandle = NULL;
TimerHandle_t drogueEjectionTimerHandle = NULL;

//  switch off drogue pyro-channel
void DrogueEjectionTimerCallback(TimerHandle_t ejectionTimerHandle)
{
    digitalWrite(DROGUE_EJECTION_PIN, LOW);
}

// switch off main pyro-channel
void MainEjectionTimerCallback(TimerHandle_t ejectionTimerHandle)
{
    digitalWrite(MAIN_EJECTION_PIN, LOW);
}

void setPyroEjectionPinModes()
{
    pinMode(DROGUE_EJECTION_PIN, OUTPUT);
    pinMode(MAIN_EJECTION_PIN, OUTPUT);
}

void ejection(uint8_t ejectionPin)
{
#if PYRO_EJECTION == true
    pyroEjection(ejectionPin)
#else
    servoEjection(ejectionPin);
#endif // PYRO_EJECTION
}

// Ejection fires the explosive charge using a mosfet
void pyroEjection(uint8_t ejectionPin)
{
    if (ejectionPin == MAIN_EJECTION_PIN)
    {
        digitalWrite(MAIN_EJECTION_PIN, HIGH);
        mainEjectionTimerHandle = xTimerCreate("MainEjectionTimer", EJECTION_PIN_ON_TIME_MS / portTICK_PERIOD_MS, pdFALSE, (void *)0, MainEjectionTimerCallback);
        xTimerStart(mainEjectionTimerHandle, portMAX_DELAY);
    }
    else if (ejectionPin == DROGUE_EJECTION_PIN)
    {
        digitalWrite(DROGUE_EJECTION_PIN, HIGH);
        drogueEjectionTimerHandle = xTimerCreate("DrogueEjectionTimer", EJECTION_PIN_ON_TIME_MS / portTICK_PERIOD_MS, pdFALSE, (void *)0, DrogueEjectionTimerCallback);
        xTimerStart(drogueEjectionTimerHandle, portMAX_DELAY);
    }
}

// Ejection with servo for water rocket
void servoEjection(uint8_t ejectionPin)
{
    if (ejectionPin == MAIN_EJECTION_PIN)
    {
        vTaskResume(MainEjectTaskHandle);
    }
    else if (ejectionPin == DROGUE_EJECTION_PIN)
    {
        vTaskResume(DrogueEjectTaskHandle);
    }
}