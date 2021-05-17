/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <string.h>
#include "stm32f1xx.h"

UART_HandleTypeDef uart;

volatile uint32_t timer_ms = 0, direction = 0, step = 0;

void delay(int time)
{
	int i;
	for(i=0; i < time * 570; i++) {};
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);	//zmien stan diody
	direction ^= 1;
}


void send_string(char *s)
{
	HAL_UART_Transmit(&uart, (uint8_t*)s, strlen(s), 1000);
}

int main(void)
{

	SystemCoreClock = 8000000;	// taktowanie 8Mhz

	HAL_Init();


	__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	//__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
	gpio.Pin = GPIO_PIN_5;	// konfigurujemy pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP;	// jako wyjœcie
	gpio.Pull = GPIO_NOPULL;	// rezystory podci¹gaj¹ce s¹ wy³¹czone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcz¹ nieskie czêstotliwoœci prze³¹czania
	HAL_GPIO_Init(GPIOA, &gpio);	// inicjalizacja modu³u GPIOA

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_2;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
			   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;	// jako wyjœcie
	gpio.Pull = GPIO_NOPULL;	// rezystory podci¹gaj¹ce s¹ wy³¹czone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcz¹ nieskie czêstotliwoœci prze³¹czania
	HAL_GPIO_Init(GPIOC, &gpio);	// inicjalizacja modu³u GPIOA

	gpio.Pin = GPIO_PIN_13; //konfigurujemy pin 13
	gpio.Mode = GPIO_MODE_IT_RISING_FALLING; ///jako wejscie
	gpio.Pull = GPIO_PULLUP;	//wlaczamy rezystro podciagajacy
	HAL_GPIO_Init(GPIOC, &gpio);	//port GPIOC



	uart.Instance = USART2;
	uart.Init.BaudRate = 115200;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);



	uint32_t led = 0;

	while (1)
	{
		send_string("Hello world!\r\n");
		HAL_Delay(100);

		HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
		HAL_Delay(150);
		HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
		if (direction == 0)
		{
			if (++led >= 10)
			{ // przejdz do nastepnej
		      led = 0;
			}
		}else
		{
			if (--led == -1)
			{ // przejdz do nastepnej
			  led = 10;
			}
		}

		/*
		if (++led >= 10) { // przejdz do nastepnej
		      led = 0;
		}
		*/

		/*
		 *
		 */
		/*
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(100);
		 */
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
