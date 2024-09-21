/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/queue.h"

#define LED GPIO_NUM_2
#define BUT GPIO_NUM_4

// Crie um handle para a  fila
QueueHandle_t xQueue;


//Task l� GPIO4 e passa o stado para a queue
void task_BUT(void * parameter)
{
	while(true)
	{
	int state = gpio_get_level(BUT);
	// Envia o estado do bot�o para a fila
    xQueueSend(xQueue, &state, portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS); 
	}
}


void task_LED(void * parameter)
{
	
	while(true)
	{
		int state_received;
		
        // Recebe o estado do bot�o da fila
        if (xQueueReceive(xQueue, &state_received, portMAX_DELAY)) 
        {
            gpio_set_level(LED, state_received);  // Liga/Desliga o LED com base no estado recebido
		}
				}
	}
	
	
void app_main(void)
{
	// Cria a fila com capacidade para armazenar 10 inteiros
    xQueue = xQueueCreate(2, sizeof(int));
	
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(BUT);
    gpio_set_direction(LED, GPIO_MODE_INPUT);
    
    xTaskCreate(&task_BUT, "task_BUT", 2024, NULL, 1, NULL);
    xTaskCreate(&task_LED, "task_LED", 2024, NULL, 1, NULL);
}
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

// Defini��o dos pinos
#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_4

// Fila para comunica��o entre tasks
QueueHandle_t buttonQueue;

// Task que l� o bot�o e envia seu estado para a fila
void button_task(void *pvParameter) {
    esp_rom_gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    
    while (1) {
        int button_state = gpio_get_level(BUTTON_PIN);
        // Envia o estado do bot�o para a fila
        xQueueSend(buttonQueue, &button_state, portMAX_DELAY);
        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay de 100ms
    }
}

// Task que recebe o estado do bot�o e controla o LED
void led_task(void *pvParameter) {
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    int button_state;
    
    while (1) {
        // Recebe o estado do bot�o da fila
        if (xQueueReceive(buttonQueue, &button_state, portMAX_DELAY)) {
            gpio_set_level(LED_PIN, button_state);  // Liga/Desliga o LED com base no estado recebido
            //vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void app_main() {
    // Cria a fila com capacidade para armazenar 10 inteiros
    buttonQueue = xQueueCreate(10, sizeof(int));
    
    // Cria as tasks para monitorar o bot�o e controlar o LED
    xTaskCreate(&button_task, "button_task", 1024, NULL, 5, NULL);
    xTaskCreate(&led_task, "led_task", 1024, NULL, 5, NULL);
}