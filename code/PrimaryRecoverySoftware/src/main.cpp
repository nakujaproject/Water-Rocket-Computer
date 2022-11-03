#include <states.h>
#include <servo.h>
#include <sensors.h>
#include <tasks.h>
#include <flash_memory.h>

TaskHandle_t GetDataTaskHandle = NULL;
TaskHandle_t DrogueEjectTaskHandle = NULL;
TaskHandle_t MainEjectTaskHandle = NULL;

const BaseType_t pro_cpu = 0;
const BaseType_t app_cpu = 1;

float base_altitude = 0;
int state = PRE_FLIGHT_GROUND_STATE;

void setup()
{
    Serial.begin(BAUD_RATE);
    
    initFlashMemory();

#if PYRO_EJECTION == true
    setPyroEjectionPinModes();
#else
    setUpEjectionServos();
    
#endif // PYRO_EJECTION

    init_bmp_mpu_sensors();

    // get the base_altitude
    base_altitude = get_base_altitude();

    // TODO: what is the velocity??
    writeFlightEventToFlash(millis(), base_altitude, 0, state);

    // initialize tasks on core 0
    xTaskCreatePinnedToCore(GetDataTask, "GetDataTask", 3000, NULL, 1, &GetDataTaskHandle, pro_cpu);

    // initialize tasks on core 1
    xTaskCreatePinnedToCore(MainEjectTask, "MainEjectTask", 1500, NULL, 1, &MainEjectTaskHandle, app_cpu);
    xTaskCreatePinnedToCore(DrogueEjectTask, "DrogueEjectTask", 1500, NULL, 1, &DrogueEjectTaskHandle, app_cpu);

    vTaskDelete(NULL);
}
void loop()
{
    vTaskDelete(NULL);
}
