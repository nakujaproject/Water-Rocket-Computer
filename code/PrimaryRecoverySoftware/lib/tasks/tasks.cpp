#include <tasks.h>
#include <defs.h>

void DrogueEjectTask(void *parameter)
{

    for (;;)
    {
        // TODO: test this
        vTaskSuspend(NULL);
        drogueServo->write(0);
    }
}
void MainEjectTask(void *parameter)
{

    for (;;)
    {
        // TODO: test this
        vTaskSuspend(NULL);
        mainServo->write(0);
    }
}

void GetDataTask(void *parameter)
{

    struct FlightData flightData = {0};

    for (;;)
    {

        flightData = readFlightData();

        // yield to idle task on core 0
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}