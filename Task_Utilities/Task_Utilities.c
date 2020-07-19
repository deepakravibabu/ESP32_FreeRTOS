//Task Details are collected (runtime, total tasks, task status,...)

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

TaskHandle_t task1handle;
TaskHandle_t task2handle;

void task1(void * pass){
	int cnt = (int *) pass;
	while(1){
		printf("Task1 exec for count...%d \r\n", cnt++);
		vTaskDelay(1000/portTICK_PERIOD_MS);
		printf("Name Task2: %s \n", pcTaskGetTaskName(task2handle)); //get taskname from taskhandler
		eTaskState myState;
		myState = eTaskGetState(task2handle); //status of the task {eRunning = 0 unto eDeleted=4}
		printf("State of Task1:\t %d \n", myState);
		vTaskSuspend(task2handle); //suspend task2 using task2handler
		printf("Tasks Running: %d\n", uxTaskGetNumberOfTasks()); //total tasks running
		printf("Tasks Ticks: %d\n", xTaskGetTickCount()); //running time of tasks based on clock ticks
	}
}

void task2(){
	while(1){
		printf("Task2 exec...\n");
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

void app_main(){
	int pass = 10;
	xTaskCreate(task1, "Task1", 2048, (void *) pass, 5, &task1handle);
	xTaskCreate(task2, "Task2", 2048, NULL, 5, &task2handle);
}

