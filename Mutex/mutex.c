//Mutex used to lock the common resource when different tasks are accessing it.
//One task takes the mutex lock, works on the resource and
//releases the mutex for the other process to use the resource


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "freertos/semphr.h" //semaphore library

TaskHandle_t tsk1Handle; //task handler creation
TaskHandle_t tsk2Handle;
SemaphoreHandle_t xmutex; //mutex handler creation


char sharedresource[4];

//task1 writes abc to the sharedresource array
void task1(){
	while(1){
		if(xSemaphoreTake(xmutex, (TickType_t) portMAX_DELAY) == 1 ){ //check if mutex lock already taken
			int i;
			char t1 [] = "abc";
				for(i=0; i<3; i++){
					sharedresource[i] = t1[i];
					vTaskDelay(1000/portTICK_PERIOD_MS);
				}
			sharedresource[i] = 0;
			printf("T1 => %s \n", sharedresource);
			xSemaphoreGive(xmutex); // release the mutex lock after use
		}
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

//task2 writes xyz to the sharedresource array
void task2(){
	while(1){
		if(xSemaphoreTake(xmutex, (TickType_t) portMAX_DELAY) == 1 ){
		int j;
		char t2 [] = "xyz";
			for(j=0; j<3; j++){
				sharedresource[j] = t2[j];
				vTaskDelay(1000/portTICK_PERIOD_MS);
			}
		sharedresource[j] = 0;
		printf("T2 => %s \n", sharedresource);
		xSemaphoreGive(xmutex);
		}
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

void app_main(){
	xmutex = xSemaphoreCreateMutex(); //mutex creation
	xTaskCreate(task1, "task1", 2048, NULL, tskIDLE_PRIORITY, &tsk1Handle);
	xTaskCreate(task2, "task2", 2048, NULL, tskIDLE_PRIORITY, &tsk2Handle);
}
