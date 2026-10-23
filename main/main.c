#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define NUM_TASKS 4

static SemaphoreHandle_t semaphores[NUM_TASKS];

static void vTask(void *pvParameters)
{
    const int taskNum = (int)(intptr_t)pvParameters;

    for (;;) {
        xSemaphoreTake(semaphores[taskNum], portMAX_DELAY);

        printf("Hello from task %d\n", taskNum + 1);

        const int nextTask = (taskNum + 1) % NUM_TASKS;
        vTaskDelay(pdMS_TO_TICKS(100));
        xSemaphoreGive(semaphores[nextTask]);
    }
}

int main(void)
{
    stdio_init_all();

    for (int i = 0; i < NUM_TASKS; i++) {
        semaphores[i] = xSemaphoreCreateBinary();

        if (semaphores[i] == NULL) {
            for (;;);
        }
    }

    for (int i = 0; i < NUM_TASKS; i++) {
        if (xTaskCreate(
                vTask,
                "Task",
                configMINIMAL_STACK_SIZE,
                (void *)(intptr_t)i,
                1,
                NULL) != pdPASS) {
            for (;;);
        }
    }

    /* Inicia a sequência: Task 1 → Task 2 → Task 3 → Task 4. */
    xSemaphoreGive(semaphores[0]);

    vTaskStartScheduler();

    for (;;);
}
