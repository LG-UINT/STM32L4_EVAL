/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

#include "uart_printf.h"
#include "peripheral.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId LedRedTaskHandle;
osThreadId LedGreenTaskHandle;
osThreadId WkUpButtonTaskHandle;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config( void );
void StartDefaultTask( void const * argument);
void vLEDRedToggle( void const * argument);
void vLEDGreenToggle( void const * argument);
void vWkUpButtonSuspendResume( void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main( void )
{

    /**
     * Reset of all peripherials, Initializes the Flash interface and Systick
     * Configure the system clock
     * Initialize the configured peripherials
     */
  vPeripherialInit();

#ifdef config_UART_PRINTF
  vUartPrintfInit();
  printf("Hello World ! \r\n");
#endif

    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */

  osThreadDef( LedRedTask, vLEDRedToggle, osPriorityNormal, 0, 128);
  osThreadDef( LedGreenTask, vLEDGreenToggle, osPriorityNormal, 0, 128);
  osThreadDef( WkUpButtonTask, vWkUpButtonSuspendResume, osPriorityNormal, 0, 128);
  //osThreadDef( defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);

  LedRedTaskHandle   = osThreadCreate(osThread(LedRedTask), NULL);
  LedGreenTaskHandle = osThreadCreate(osThread(LedGreenTask), NULL);
  WkUpButtonTaskHandle = osThreadCreate(osThread(WkUpButtonTask), NULL);
  //defaultTaskHandle  = osThreadCreate(osThread(defaultTask), NULL);


    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */


    /* Start scheduler */
  osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
  while (1)
  {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

  }
    /* USER CODE END 3 */

}


/* StartDefaultTask function */
void StartDefaultTask( void const *argument)
{

    /* USER CODE BEGIN 5 */
    /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
    /* USER CODE END 5 */
}

/**
 * [vLEDRedToggle Toggle Led Red]
 * @param argument [description]
 */
void vLEDRedToggle( void const *argument)
{
  while(1)
  {
    LED_RED_TOGGLE;
    printf("Led Red Toggle Task Runs. \r\n");
    osDelay(1000);
  }
}

void vLEDGreenToggle( void const *argument)
{
  while(1)
  {
    LED_GREEN_TOGGLE;
    printf("Led Green Toggle Task Runs. \r\n");
    osDelay(2000);
  }

}


void vWkUpButtonSuspendResume( void const *argument)
{

  while(1)
  {

    if( WK_UP_BUTTON_DETECT() == WK_UP_BUTTON_PRESS )
    {
      /* Debounced */
      osDelay(200);
      if( osThreadIsSuspended(LedRedTaskHandle) != osOK )
      {
        if( osThreadSuspend(LedRedTaskHandle) == osOK )
        {
          printf("Led Red Task Suspended...\r\n");
        }

      }
      else
      {
        if( osThreadResume( LedRedTaskHandle) == osOK )
        {
          printf("Led Red Task Resumed...\r\n");
        }
      }
    }
  }

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
