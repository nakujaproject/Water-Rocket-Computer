#ifndef TASKS_H
#define TASKS_H
#include <defs.h>
#include <servo.h>
#include <flight_data.h>

void MainEjectTask(void *parameter);
void DrogueEjectTask(void *parameter);
void GetDataTask(void *parameter);

#endif // TASKS_H