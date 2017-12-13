/**
  ******************************************************************************
  * File Name          : uart_printf.c
  * Description        : Printf remapping uart function
  * Date               : 2017/12/12
  * Author             : Guang LI
  * Email        : Guang.LI@uint.fr
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 UINT All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of UINT nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. Redistribution and use of this software other than as permitted under
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

#include "uart_printf.h"
#include "stm32l4xx_hal.h"


/**
 * Uart printf Macros
 */
#define UART_PRINT            LPUART1
#define UART_PRINT_BDR        115200
#define UART_PRINT_TX         GPIO_PIN_7
#define UART_PRINT_TX_PORT    GPIOG
#define UART_PRINT_RX         GPIO_PIN_8
#define UART_PRINT_RX_PORT    GPIOG
#define UART_PRINT_Alternate  GPIO_AF8_LPUART1


/* Uart printf word length */
#define UART_PRINT_WDLEN     UART_WORDLENGTH_7B

/* Uart printf stop bit */
#define UART_PRINT_STBIT     UART_STOPBITS_1

/* Uart printf partity */
#define UART_PRINT_PARTITY   UART_PARITY_NONE

/* Uart printf mode */
#define UART_PRINT_MODE      UART_MODE_TX_RX

#define UART_PRINT_TX_PORT_CLK_ENABLE()   __HAL_RCC_GPIOG_CLK_ENABLE()
#define UART_PRINT_RX_PORT_CLK_ENABLE()   __HAL_RCC_GPIOG_CLK_ENABLE()
#define UART_PRINT_TX_PORT_CLK_DISABLE()  __HAL_RCC_GPIOG_CLK_DISABLE()
#define UART_PRINT_RX_PORT_CLK_DISABLE()  __HAL_RCC_GPIOG_CLK_DISABLE()
#define UART_PRINT_CLK_ENALE()            __HAL_RCC_LPUART1_CLK_ENABLE()
#define UART_PRINT_CLK_DISABLE()          __HAL_RCC_LPUART1_CLK_DISABLE()


UART_HandleTypeDef hlpuart1;

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar( int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART1 and Loop until the end of transmission */
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}


/* LPUART1 init function */
void vUartPrintfInit( void )
{

    hlpuart1.Instance                    = UART_PRINT;
    hlpuart1.Init.BaudRate               = UART_PRINT_BDR;
    hlpuart1.Init.WordLength             = UART_PRINT_WDLEN;
    hlpuart1.Init.StopBits               = UART_PRINT_STBIT;
    hlpuart1.Init.Parity                 = UART_PRINT_PARTITY;
    hlpuart1.Init.Mode                   = UART_PRINT_MODE;
    hlpuart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    hlpuart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&hlpuart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

}

void vUartPrintfMspInit( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    UART_PRINT_CLK_ENALE();
    UART_PRINT_TX_PORT_CLK_ENABLE();
    UART_PRINT_RX_PORT_CLK_ENABLE();

#if ( VDDIO2 == 1 )
    HAL_PWREx_EnableVddIO2();
#endif
    /**
      * Uart printf GPIO Configuration
    */
    GPIO_InitStruct.Pin       = UART_PRINT_TX;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(UART_PRINT_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = UART_PRINT_RX;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(UART_PRINT_RX_PORT, &GPIO_InitStruct);

}


void vUartPrintfDeMspInit( void )
{

    UART_PRINT_CLK_DISABLE();
    UART_PRINT_TX_PORT_CLK_DISABLE();
    UART_PRINT_RX_PORT_CLK_DISABLE();

#if ( VDDIO2 == 1 )
    HAL_PWREx_DisableVddIO2();
#endif

    HAL_GPIO_DeInit(UART_PRINT_TX_PORT, UART_PRINT_TX);
    HAL_GPIO_DeInit(UART_PRINT_RX_PORT, UART_PRINT_RX);
}

/************************ (C) COPYRIGHT UINT *********END OF FILE*************/