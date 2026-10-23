#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h

static void vHelloTask(void *pvParameters) {
    (void)pvParameters;

    for (;;) {
        printf("Hello from task!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    stdio_init_all();

    if (xTaskCreate(
            vHelloTask,
            "Hello",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL) != pdPASS) {
        for (;;);
    }

    vTaskStartScheduler();

    for (;;);
}
