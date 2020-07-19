//Queue for transfer of data between tasks

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
QueueHandle_t myQueue;

void task1(void *pvParameter)
{
	char Txbuff[40];
	myQueue = xQueueCreate(20, sizeof(Txbuff)); //creation of queue

	sprintf(Txbuff, "msg1");
	xQueueSendToFront(myQueue, (void *) Txbuff, (TickType_t)0); //puts the msg at front of the queue
	sprintf(Txbuff, "msg2");
	xQueueSendToFront(myQueue, (void *) Txbuff, (TickType_t)0);

	sprintf(Txbuff, "msg3");
	xQueueSend(myQueue, (void *) Txbuff, (TickType_t)0); //puts the msg at the end of queue
	sprintf(Txbuff, "msg4");
	xQueueSend(myQueue, (void *) Txbuff, (TickType_t)0);
	sprintf(Txbuff, "msg5");
	xQueueSend(myQueue, (void *) Txbuff, (TickType_t)0);

	while(1) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void task2(void *pvParameter)
{
	char Rxbuff[40];
	while(1) {
		if(myQueue != 0){
			if(xQueueReceive(myQueue, (void *)Rxbuff, (TickType_t)5)){ //reads the queue and store it in Rxbuff array
				printf("Rxbuff:::%s\n",Rxbuff); 					   //ticktype 5 is blocking time for task2 while queue is being read
				printf("Data waiting in Queue: %d \n", uxQueueMessagesWaiting(myQueue));
				printf("Available Space in Queue: %d \n", uxQueueSpacesAvailable(myQueue));
			}
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main()
{
	xTaskCreate(task1, "Task1", 2048, (void *) 0, 5, &myTask1Handle);
	xTaskCreate(task2, "Task2", 2048, (void *) 0, 5, &myTask2Handle);
}
