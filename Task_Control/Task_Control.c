/* Suspending and Resuming Task */


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

void task1(void *pvParameter)
{
	int count = (int *) pvParameter ;

	while(1) {
		vTaskDelay(1000 / portTICK_PERIOD_MS); //delay of 1 second
		printf("Task1 time: \t %d s \n", count ++);
	}
}

void task2(void *pvParameter)
{
	while(1) {
		vTaskDelay(5000 / portTICK_PERIOD_MS); //delay of 5 second
		vTaskSuspend(myTask1Handle);
		printf("T2 => task1 Suspended \n");
		vTaskDelay(5000 / portTICK_PERIOD_MS); //delay of 5 second
		vTaskResume(myTask1Handle);
		printf("T2 => task1 Resume \n");
	}
}


void app_main(){

	xTaskCreate(&task1, "Task1", 2048, (void *) 0, 5, &myTask1Handle);
	xTaskCreate(&task2, "Task2", 2048, (void *) 0, 5, &myTask2Handle);
}

