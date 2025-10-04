/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include <string.h>
#include "queue.h"
#include "timers.h"
#include "task.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
	uint8_t cmd[10];
	int len;
}cmd_variable;


typedef enum
{
	sMainMenu=0,
	sLedEffect,
	sRtcMenu,
	sRtcTimeConfig,
	sRtcDateConfig,
}state_t;


extern xQueueHandle q_enqueue;
extern xQueueHandle q_print;

extern volatile uint8_t pData;
extern TimerHandle_t timer_handle[4];
extern state_t curr_state ;
extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

extern xTaskHandle xHandlemenu;
extern xTaskHandle xHandlecmd_handler;
extern xTaskHandle xHandleprint_task ;
extern xTaskHandle xHandleled_task ;
extern xTaskHandle xHandlertc_task ;

extern int flag1 ;
extern int flag2 ;
extern int flag3 ;
extern int flag4 ;
extern int help ;


extern volatile int rtc_state;
extern volatile int rtc_state_date;




/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void led_task(void* pv);
void led_effect_stop(void);
void led_effect(int i);
void led_effect1(void);
void led_effect2(void);
void led_effect3(void);
void led_effect4(void);
void menu_function(void* pv);
void rtc_task(void* pv);
void cmd_handler_function(void* pv);
void print_task(void* pv);
void rtc_configure_time(RTC_TimeTypeDef *time);
void rtc_configure_date(RTC_DateTypeDef *date);
void show_time_date(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_5
#define LED4_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_8
#define LED2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
