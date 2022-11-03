#ifndef FLASHMEMORY_H
#define FLASHMEMORY_H

#include <defs.h>
#include "FS.h"
#include <LITTLEFS.h>

void initFlashMemory();
void appendFile(fs::FS &fs, const char *path, const char *message);
char *printFlashMessage(int time, float altitude, float velocity, int state);
void writeFlightEventToFlash(int time, float altitude, float velocity, int state);
void readFile(fs::FS &fs, const char *path);

#endif // FLASHMEMORY_H