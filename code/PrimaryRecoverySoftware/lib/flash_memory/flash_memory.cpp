#include <flash_memory.h>

#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
#include <time.h>
#endif // CONFIG_LITTLEFS_FOR_IDF_3_2

#define FORMAT_LITTLEFS_IF_FAILED false

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        debugln("Open Failed");
        return;
    }
    if (file.print(message))
    {
        debugln("Message appended");
    }
    else
    {
        debugln("Append failed");
    }
    file.close();
}

void readFile(fs::FS &fs, const char *path)
{
    debugf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        debugln("- failed to open file for reading");
        return;
    }

    debugln("- read from file:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void initFlashMemory()
{
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        debugln("LITTLEFS Mount Failed");
        return;
    }

    appendFile(LITTLEFS, "/flights/flight_data.txt", "New Flight\n");
}
char *printFlashMessage(int time, float altitude, float velocity, int state)
{
    // The assigned size is calculated to fit the string
    char *message = (char *)pvPortMalloc(50);

    if (!message)
    {
        return NULL;
    }

    snprintf(message, 50, "t:%d,ASL:%.3f,v:%.3f,state: %d\n", time, altitude, velocity, state);
    return message;
}
void writeFlightEventToFlash(int time, float altitude, float velocity, int state)
{
    char *message = printFlashMessage(time, altitude, velocity, state);
    appendFile(LITTLEFS, "/flights/flight_data.txt", message);
    vPortFree(message);
    //readFile(LITTLEFS, "/flights/flight_data.txt");
}
