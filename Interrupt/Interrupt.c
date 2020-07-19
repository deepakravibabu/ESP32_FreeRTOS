// Interrupt task is created which executes upon button press to switch the internal LED

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define CONFIG_LED_PIN 13 //inbuilt LED at pin 13
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0 //inbuilt button at pin 0


TaskHandle_t ISR = NULL;
TaskHandle_t task1 = NULL;


// interrupt service routine, called when the button is pressed
void IRAM_ATTR button_isr_handler(void *arg) {

	xTaskResumeFromISR(ISR); //task resume called from within ISR to button task
//portYIELD_FROM_ISR(  );
}

//task executing always
void disp_task (void *p){
	while (1) {
		printf("Press the button...\n");
		vTaskDelay(10 * portTICK_PERIOD_MS); //delay of 1 second (portTICK_PERIOD_MS= 10msec)

	}
}

// task that will react to button clicks
void button_task(void *arg) {
	bool led_status = false;
	while (1) {
		vTaskSuspend(NULL); //button task is suspend always and executed only upon interrupt
		led_status = !led_status;
		gpio_set_level(CONFIG_LED_PIN, led_status); //set ledpin to high
		printf("Button pressed!!!\n");
	}
}

void app_main() {

	gpio_pad_select_gpio(CONFIG_BUTTON_PIN); //define the pins used
	gpio_pad_select_gpio(CONFIG_LED_PIN);

	// set the correct direction
	gpio_set_direction(CONFIG_BUTTON_PIN, GPIO_MODE_INPUT);
	gpio_set_direction(CONFIG_LED_PIN, GPIO_MODE_OUTPUT);

	// enable interrupt on falling (1->0) edge for button pin
	gpio_set_intr_type(CONFIG_BUTTON_PIN, GPIO_INTR_NEGEDGE);

	// install ISR service with default configuration
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

	// attach the interrupt service routine
	gpio_isr_handler_add(CONFIG_BUTTON_PIN, button_isr_handler, NULL);

	//Create and start stats task

	xTaskCreate(button_task, "button_task", 4096, NULL, 10, &ISR);
	xTaskCreate(disp_task, "task1", 4096, NULL, 10, &task1);


}
