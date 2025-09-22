#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

/* Semaphores */
SemaphoreHandle_t semaphores[4];

/* Task function */
void vTask(void *pvParameters)
{
    int taskNum = (int)pvParameters;

    for (;;)
    {
        // Wait for my semaphore
        xSemaphoreTake(semaphores[taskNum], portMAX_DELAY);

        // Critical section: do my work
        printf("Hello from task %d\n", taskNum + 1);

        // Release next task’s semaphore
        int nextTask = (taskNum + 1) % 4;
        xSemaphoreGive(semaphores[nextTask]);
    }
}

int main(void)
{
    for (int i = 0; i < 4; i++)
    {
        semaphores[i] = xSemaphoreCreateBinary();
    }

    for (int i = 0; i < 4; i++)
    {
        char name[10];
        snprintf(name, sizeof(name), "Task%d", i + 1);
        xTaskCreate(vTask, name, configMINIMAL_STACK_SIZE, (void *)i, 1, NULL);
    }

    xSemaphoreGive(semaphores[0]);

    vTaskStartScheduler();

    // Should never reach here
    for (;;);
}
