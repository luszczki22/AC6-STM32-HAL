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

	SystemCoreClock = 8000000;	// taktowanie 8Mhz

	HAL_Init();


	__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	//__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
	gpio.Pin = GPIO_PIN_5;	// konfigurujemy pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP;	// jako wyjœcie
	gpio.Pull = GPIO_NOPULL;	// rezystory podci¹gaj¹ce s¹ wy³¹czone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcz¹ nieskie czêstotliwoœci prze³¹czania
	HAL_GPIO_Init(GPIOA, &gpio);	// inicjalizacja modu³u GPIOA

	gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
			 GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;	// jako wyjœcie
	gpio.Pull = GPIO_NOPULL;	// rezystory podci¹gaj¹ce s¹ wy³¹czone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcz¹ nieskie czêstotliwoœci prze³¹czania
	HAL_GPIO_Init(GPIOC, &gpio);	// inicjalizacja modu³u GPIOA

	gpio.Pin = GPIO_PIN_13; //konfigurujemy pin 13
	gpio.Mode = GPIO_MODE_INPUT; ///jako wejscie
	gpio.Pull = GPIO_PULLUP;	//wlaczamy rezystro podciagajacy
	HAL_GPIO_Init(GPIOC, &gpio);	//port GPIOC

	uint32_t led = 0;

	while (1)
	{
		HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET);
		HAL_Delay(150);
		HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET);
		if(++led >= 10) // przejdz do nastepnej
		{
			led = 0;
		}


		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(100);

	}




	/*
	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_RESET) //// jesli przycisk jest przycisniety,
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	//zapal diode
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		}

	}
	*/

	/*
	while(1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);
		delay(400);
	}
	*/

}
