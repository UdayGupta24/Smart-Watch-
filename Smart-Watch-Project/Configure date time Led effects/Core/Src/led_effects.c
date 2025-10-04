#include "main.h"


void led_effect_stop(void)
{
	for(int i=0;i<4;i++)
		xTimerStop(timer_handle[i],portMAX_DELAY);
}

void led_effect(int i)
{
	led_effect_stop();
	xTimerStart(timer_handle[i-1],portMAX_DELAY);
}
void led_effect1(void)
{
	if(flag1 ==1)
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_RESET);
	}
	flag1 =flag1^1;
}
void led_effect2(void)
{
	if(flag2 ==1)
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_SET);
	}
	flag2 =flag2^1;
}
void led_effect3(void)
{
	if(flag1 ==1)
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_SET);
	}
	flag1 =flag1^1;
}
void led_effect4(void)
{
	if(flag4 ==1)
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_RESET);
	}
	else if(flag4 ==2)
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_RESET);
	}
	else if(flag4 ==3)
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_SET);
	}
	flag4 =(flag4+1)%4;
}
