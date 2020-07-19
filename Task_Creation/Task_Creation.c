/* Switching between two tasks in a specified delay time;
 Precise Delay time in terms of clock ticks (using VtaskDelayUntil) */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t myTaskHandle = NULL;

void task1(void *pvParameter)
{
	int count = (int *) pvParameter ;
	TickType_t myLastUnblock;
	myLastUnblock = xTaskGetTickCount();

	while(1) {
		printf("Task1 exec...%d \n", count ++);
		//vTaskDelay(10000 / portTICK_PERIOD_MS);
		vTaskDelayUntil(&myLastUnblock, 1000 / portTICK_PERIOD_MS);

		if (count == 27){ 				//when count becomes 27 the task1 gets deleted
			vTaskDelete(myTaskHandle);
		}
	}
}

void task2(void *pvParameter)
{
	while(1) {
		printf("Task2 exec...\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main()
{
	int pass = 25; //variable passed to task1
	xTaskCreate(&task1, "Task1", 2048, (void *) pass, 5, NULL);
	xTaskCreate(&task2, "Task2", 2048, NULL, 5, NULL);
}

