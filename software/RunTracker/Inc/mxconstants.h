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

#define OLED_EN_Pin GPIO_PIN_13
#define OLED_EN_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOC
#define SW_TR_Pin GPIO_PIN_1
#define SW_TR_GPIO_Port GPIOC
#define SW_TL_Pin GPIO_PIN_2
#define SW_TL_GPIO_Port GPIOC
#define SW_BR_Pin GPIO_PIN_3
#define SW_BR_GPIO_Port GPIOC
#define CENTER_SW_Pin GPIO_PIN_0
#define CENTER_SW_GPIO_Port GPIOA
#define BARO_CS_Pin GPIO_PIN_1
#define BARO_CS_GPIO_Port GPIOA
#define GPS_UART_TX_Pin GPIO_PIN_2
#define GPS_UART_TX_GPIO_Port GPIOA
#define GPS_UART_RX_Pin GPIO_PIN_3
#define GPS_UART_RX_GPIO_Port GPIOA
#define SW_RIGHT_Pin GPIO_PIN_4
#define SW_RIGHT_GPIO_Port GPIOA
#define SW_DOWN_Pin GPIO_PIN_5
#define SW_DOWN_GPIO_Port GPIOA
#define SW_LEFT_Pin GPIO_PIN_6
#define SW_LEFT_GPIO_Port GPIOA
#define SW_UP_Pin GPIO_PIN_7
#define SW_UP_GPIO_Port GPIOA
#define SW_BL_Pin GPIO_PIN_4
#define SW_BL_GPIO_Port GPIOC
#define ACCEL_INT1_Pin GPIO_PIN_5
#define ACCEL_INT1_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_0
#define OLED_DC_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_1
#define OLED_CS_GPIO_Port GPIOB
#define ACCEL_CS_Pin GPIO_PIN_2
#define ACCEL_CS_GPIO_Port GPIOB
#define VS1053_DCS_Pin GPIO_PIN_10
#define VS1053_DCS_GPIO_Port GPIOB
#define VS1053_CS_Pin GPIO_PIN_12
#define VS1053_CS_GPIO_Port GPIOB
#define VS1053_SCK_Pin GPIO_PIN_13
#define VS1053_SCK_GPIO_Port GPIOB
#define VS1053_MISO_Pin GPIO_PIN_14
#define VS1053_MISO_GPIO_Port GPIOB
#define VS1053_MOSI_Pin GPIO_PIN_15
#define VS1053_MOSI_GPIO_Port GPIOB
#define ACCEL_INT2_Pin GPIO_PIN_6
#define ACCEL_INT2_GPIO_Port GPIOC
#define VS1053_DREQ_Pin GPIO_PIN_7
#define VS1053_DREQ_GPIO_Port GPIOC
#define SW_HEADPHONE_Pin GPIO_PIN_8
#define SW_HEADPHONE_GPIO_Port GPIOA
#define BT_UART_TX_Pin GPIO_PIN_9
#define BT_UART_TX_GPIO_Port GPIOA
#define BT_UART_RX_Pin GPIO_PIN_10
#define BT_UART_RX_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_15
#define OLED_RST_GPIO_Port GPIOA
#define BT_EN_Pin GPIO_PIN_6
#define BT_EN_GPIO_Port GPIOB
#define BT_RST_Pin GPIO_PIN_7
#define BT_RST_GPIO_Port GPIOB
#define GPS_RST_Pin GPIO_PIN_8
#define GPS_RST_GPIO_Port GPIOB
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
