/*
#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_system.h"

#define INPUT_GPIO_NO 34  
#define BLINK_GPIO 2
unsigned int GPIOlevel;
static uint8_t s_led_state = 0;
static void blink_led(void)
{
   //Set the GPIO level according to the state (LOW or HIGH)
    gpio_set_level(BLINK_GPIO, s_led_state);
   
}

static void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    // Set the GPIO as a push/pull output 
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
 
}

void blinktask(void *pvParameter)
{
        gpio_pad_select_gpio(INPUT_GPIO_NO);
    gpio_pullup_dis(INPUT_GPIO_NO);
    gpio_set_direction(INPUT_GPIO_NO, GPIO_MODE_INPUT);
    GPIOlevel = gpio_get_level(INPUT_GPIO_NO);
    while(1)
    {
	if (GPIOlevel == 1)
		{
			printf("GPIO Level is HIGH !!!\n");
              configure_led();
              s_led_state=1;
              blink_led();
              vTaskDelay(5000);
		}
		else
        {		  	
                     s_led_state=0;
             blink_led();
			 printf("GPIO Level is LOW \n");   
             vTaskDelay(1000 / portTICK_RATE_MS);
        }
    }
}
void app_main()
{
     //task 1///////
     xTaskCreate(&blinktask,"blink_task",2048,NULL,5,NULL);
  
}
*/
#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "freertos/queue.h"

xQueueHandle q;

#define BLINK_GPIO 2
 #define INPUT_GPIO_NO 34
 unsigned int GPIOlevel; 
 
void blink_task(void *pvParameter)
{
 
	while(1)
	{
	    printf("Hello world!\n");
        xQueueReceive(q,&GPIOlevel,(TickType_t )10); 
        if(xQueueReceive(q,&GPIOlevel,(TickType_t )10) ==pdTRUE)
        {
        gpio_set_level(BLINK_GPIO,GPIOlevel);
	    vTaskDelay(100 / portTICK_RATE_MS);
        }
        else
        {
         printf("not recieved");
        }
	}
}
 
void button_task(void *pvParameter)
{
 ////////////////led//////////////
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    //////////////button///////////////
    gpio_pad_select_gpio(INPUT_GPIO_NO);
    gpio_pullup_dis(INPUT_GPIO_NO);
    gpio_set_direction(INPUT_GPIO_NO, GPIO_MODE_INPUT);
    
    while(1) {
        GPIOlevel = gpio_get_level(INPUT_GPIO_NO);
       if(GPIOlevel==1)
       {
         xQueueSend(q,(void *)&GPIOlevel,(TickType_t )10); 
       }
       else
       {
        gpio_set_level(BLINK_GPIO, 0);
       }
         vTaskDelay(100 / portTICK_RATE_MS);
    }
}
 
 
void app_main()
{
    q = xQueueCreate( 1, sizeof(int));
    xTaskCreate(&blink_task, "blinky", 2048, NULL, 5, NULL);
    xTaskCreate(&button_task, "button", 512,NULL,5,NULL );
}