/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SW_TR_Pin GPIO_PIN_1
#define SW_TR_GPIO_Port GPIOC
#define SW_TL_Pin GPIO_PIN_2
#define SW_TL_GPIO_Port GPIOC
#define SW_BR_Pin GPIO_PIN_3
#define SW_BR_GPIO_Port GPIOC
#define CENTER_SW_Pin GPIO_PIN_0
#define CENTER_SW_GPIO_Port GPIOA
#define GPS_UART_RX_Pin GPIO_PIN_2
#define GPS_UART_RX_GPIO_Port GPIOA
#define GPS_UART_TX_Pin GPIO_PIN_3
#define GPS_UART_TX_GPIO_Port GPIOA
#define SW_DOWN_Pin GPIO_PIN_5
#define SW_DOWN_GPIO_Port GPIOA
#define SW_LEFT_Pin GPIO_PIN_6
#define SW_LEFT_GPIO_Port GPIOA
#define SW_UP_Pin GPIO_PIN_7
#define SW_UP_GPIO_Port GPIOA
#define SW_BL_Pin GPIO_PIN_4
#define SW_BL_GPIO_Port GPIOC
#define SW_HEADPHONE_Pin GPIO_PIN_8
#define SW_HEADPHONE_GPIO_Port GPIOA
#define VS1053_RST_Pin GPIO_PIN_9
#define VS1053_RST_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
