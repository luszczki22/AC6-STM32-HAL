/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
/*//Check your linker flags (project properties > C/C++ Build > Setings > Tool Settings (TAB) > MCU GCC Linker > Miscellaneous > Linker flags),
//if you are using nanolib “-specs=nano.specs” you need to add “-u _printf_float” to enable float printf. */
#include <string.h>
#include "stm32f1xx.h"

UART_HandleTypeDef uart;
TIM_HandleTypeDef tim2;
TIM_HandleTypeDef tim4;

volatile uint32_t timer_ms = 0, direction = 0, step = 0;
int counter = 0;

void delay(int time)
{
	int i;
	for(i=0; i < time * 570; i++) {};
}

int __io_putchar(int ch)
{
	//if(ch == '\n')
	//	send_char('\r');
	send_char(ch);
	return ch;
}

void send_char(char c)
{
	HAL_UART_Transmit(&uart, (uint8_t*)&c, 1, 1000);
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

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == GPIO_PIN_RESET)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	switch(htim->Channel){
	case HAL_TIM_ACTIVE_CHANNEL_1:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_3:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_4:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}

float calc_pwm(float val)
{
	const float k = 0.1f;
	const float x0 = 60.0f;
	return 300.0f/ (1.0f + exp(-k * val));
}

int main(void)
{
	SystemCoreClock = 8000000;	// taktowanie 8Mhz
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	//__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

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

	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pin = GPIO_PIN_0;
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

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio);

	uart.Instance = USART2;
	uart.Init.BaudRate = 115200;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	RCC_PeriphCLKInitTypeDef adc_clk;
	adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV2;
	HAL_RCCEx_PeriphCLKConfig(&adc_clk);

	ADC_HandleTypeDef adc;
	adc.Instance = ADC1;
	adc.Init.ContinuousConvMode = ENABLE;
	adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	adc.Init.NbrOfConversion = 1;
	adc.Init.DiscontinuousConvMode = DISABLE;
	adc.Init.NbrOfDiscConversion = 1;
	HAL_ADC_Init(&adc);

	HAL_ADCEx_Calibration_Start(&adc);

	ADC_ChannelConfTypeDef adc_ch;
	adc_ch.Channel = ADC_CHANNEL_0;
	adc_ch.Rank = ADC_REGULAR_RANK_1;
	adc_ch.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	HAL_ADC_Start(&adc);

	tim2.Instance = TIM2;
	tim2.Init.Period = 2000 - 1;
	tim2.Init.Prescaler = 800 - 1;
	tim2.Init.ClockDivision = 0;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.RepetitionCounter = 0;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);

	tim4.Instance = TIM4;
	tim4.Init.Period = 1000 - 1;
	tim4.Init.Prescaler = 8000 - 1;
	tim4.Init.ClockDivision = 0;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.RepetitionCounter = 0;
	tim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_PWM_Init(&tim4);


	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Base_Start_IT(&tim2);

	//__HAL_TIM_SET_COMPARE(&tim2, TIM_CHANNEL_1, 100);
	//__HAL_TIM_SET_COMPARE(&tim2, TIM_CHANNEL_2, 200);
	//__HAL_TIM_SET_COMPARE(&tim2, TIM_CHANNEL_3, 500);
	//__HAL_TIM_SET_COMPARE(&tim2, TIM_CHANNEL_4, 900);

	TIM_OC_InitTypeDef oc;
	oc.OCMode = TIM_OCMODE_TIMING;
	oc.Pulse = 50;
	oc.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc.OCNPolarity =TIM_OCNPOLARITY_LOW;
	oc.OCFastMode = TIM_OCFAST_ENABLE;
	oc.OCIdleState = TIM_OCIDLESTATE_SET;
	oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_OC_ConfigChannel(&tim2, &oc, TIM_CHANNEL_1);

	oc.Pulse = 100;
	HAL_TIM_OC_ConfigChannel(&tim2, &oc, TIM_CHANNEL_2);

	oc.Pulse  = 400;
	HAL_TIM_OC_ConfigChannel(&tim2, &oc, TIM_CHANNEL_3);

	oc.Pulse = 900;
	HAL_TIM_OC_ConfigChannel(&tim2, &oc, TIM_CHANNEL_4);

	__HAL_TIM_ENABLE_IT(&tim2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4);


	TIM_OC_InitTypeDef oc2;
	oc2.OCMode = TIM_OCMODE_PWM1;
	oc2.Pulse = 100;
	oc2.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc2.OCNPolarity = TIM_OCNPOLARITY_LOW;
	oc2.OCFastMode = TIM_OCFAST_ENABLE;
	oc2.OCIdleState = TIM_OCIDLESTATE_RESET;
	oc2.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&tim4, &oc2, TIM_CHANNEL_1);

	oc2.Pulse = 500;
	HAL_TIM_PWM_ConfigChannel(&tim4, &oc2, TIM_CHANNEL_2);

	oc2.Pulse = 800;
	HAL_TIM_PWM_ConfigChannel(&tim4, &oc2, TIM_CHANNEL_3);

	oc2.Pulse = 900;
	HAL_TIM_PWM_ConfigChannel(&tim4, &oc2, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&tim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&tim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim4, TIM_CHANNEL_4);


	uint32_t led = 0;

	while (1)
	{
		uint32_t value = HAL_ADC_GetValue(&adc);
		printf("Adc = %ld (%.3fV)\r\n", value, value * 3.3f / 4095.0f);

		if (__HAL_UART_GET_FLAG(&uart, UART_FLAG_RXNE) == SET)
		{
			uint8_t value;
			HAL_UART_Receive(&uart, &value, 1, 100);

			switch (value)
			{
			case 'a':
				send_string("Recived a\r\n");
				break;
			case 'b':
				send_string("Recived b\r\n");
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
				break;
			case 'c':
				send_string("Recived c\r\n");
			}

		}

		float r = 50 * (1.0f + sin(counter / 200.0f));
		float g = 50 * (1.0f + sin(1.5f * counter / 100.0f));
		float b = 50 * (1.0f + sin(2.0f * counter / 100.0f));
		__HAL_TIM_SET_COMPARE(&tim4, TIM_CHANNEL_1, calc_pwm(b));
		__HAL_TIM_SET_COMPARE(&tim4, TIM_CHANNEL_2, calc_pwm(g));
		__HAL_TIM_SET_COMPARE(&tim4, TIM_CHANNEL_3, calc_pwm(r));

		HAL_Delay(20);
		counter++;


		/*
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
		*/

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
