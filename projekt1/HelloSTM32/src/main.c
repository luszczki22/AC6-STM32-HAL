/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f1xx.h"

void delay(int time)
{
	int i;
	for(i=0; i < time * 570; i++) {};
}


int main(void)
{

	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpio; // obiekt gpio b�d�cy konfiguracj� port�w GPIO
	gpio.Pin= GPIO_PIN_5;	// konfigurujemy pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP;	// jako wyj�cie
	gpio.Pull = GPIO_NOPULL;	// rezystory podci�gaj�ce s� wy��czone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcz� nieskie cz�stotliwo�ci prze��czania
	HAL_GPIO_Init(GPIOA, &gpio);	// inicjalizacja modu�u GPIOA

	while(1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);
		delay(400);
	}


}
