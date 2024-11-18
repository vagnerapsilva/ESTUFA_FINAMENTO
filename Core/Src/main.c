/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "DHT.h"
#include "delays.h"
#include "gifs.h"
#include "ssd1306.h"
#include "fonts.h"
#include "bitmaps.h"
#include "temperatura.h"
#include "menu.h"

#define ARM_MATH_CM4
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
DHT_DataTypedef DHT11_Data;
float TempC = 0;
float TempC_Trab = 33.5;
extern uint32_t cont_tempo_troca_ar;
extern uint8_t cont_tempo_fan_on;

uint16_t Tempo_Trab = 0;
uint16_t Tempo_epoch = 0;
float Humidity = 0;
char MSG[50];
uint32_t Encoder_valor;
extern float duty_s;
extern float fan_s;
extern uint8_t em_processo;
uint8_t perfil = 0;

uint8_t fan1=0;
uint8_t fan2=0;
uint8_t fan3=0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM17_Init();
  MX_I2C2_Init();
  MX_CRC_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim17);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

	SSD1306_Init();

	htim2.Instance->CCR1 = (htim2.Init.Period);
	Temperatura_Init();
	set_material();
	htim2.Instance->CCR1 =0;
	htim2.Instance->CCR2 =0;
	htim2.Instance->CCR3 =0;
	htim2.Instance->CCR4 =0;

	/*		while (1)
	 {
	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
	 DelayUs(10);
	 }*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
			Error_Handler();
		}

		if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
			Error_Handler();
		}

		if (key_press() == 0) {
			htim2.Instance->CCR1 = 0;
			htim2.Instance->CCR2 = 0;
			htim2.Instance->CCR3 = 0;
			htim2.Instance->CCR4 = 0;
			em_processo = menu(&perfil);
		}
		if (em_processo == 1) {

			Tempo_epoch = (sTime.Hours * 60) + sTime.Minutes;
			Tempo_Trab = (Filament[perfil].TEMPO) - Tempo_epoch;

			if (Tempo_Trab == 0)
				HAL_NVIC_SystemReset(); //Reset

			DHT_GetData(&DHT11_Data);

			TempC = DHT11_Data.Temperature;
			Humidity = DHT11_Data.Humidity;
			TempC_Trab = Filament[perfil].TEMP;

			//liga fan1
			fan1 = 100;

			Temperatura();

			SSD1306_GotoXY(5, 18);
			sprintf(MSG, "T(%.2u) H(%.2u)", (int) TempC, (int) Humidity);
			SSD1306_Puts(MSG, &Font_11x18, 1);

			SSD1306_GotoXY(1, 3);
			sprintf(MSG, "%s:%.2d:%.2d-C:%.2u", Filament[perfil].MATERIAL,
					(Tempo_Trab/60),(Tempo_Trab%60), (int)TempC_Trab); //C%.2f, TempC_Trab);
			SSD1306_Puts(MSG, &Font_7x10, 1);

//			SSD1306_GotoXY(10, 40);
//			sprintf(MSG, "RS.%%%.3d F1.%%%.3d", (int) (duty_s),
//					(int) ((htim2.Instance->CCR2 * 100) / htim2.Init.Period));
//			SSD1306_Puts(MSG, &Font_7x10, 1);
//
//			SSD1306_GotoXY(10, 53);
//			sprintf(MSG, "F2.%%%.3d F3.%%%.3d ",
//					(int) ((htim2.Instance->CCR3 * 100) / htim2.Init.Period),
//					(int) ((htim2.Instance->CCR4 * 100) / htim2.Init.Period));
//			SSD1306_Puts(MSG, &Font_7x10, 1);

			SSD1306_GotoXY(10, 40);
			sprintf(MSG, "DT%%%.3d-F%.1d-I%1d-E%.1d", (int) (duty_s),
					(int) (((htim2.Instance->CCR2 * 100) / htim2.Init.Period)/100),
					(int) (((htim2.Instance->CCR4 * 100) / htim2.Init.Period)/100),
					(int) (((htim2.Instance->CCR3 * 100) / htim2.Init.Period)/100));
			SSD1306_Puts(MSG, &Font_7x10, 1);

			SSD1306_GotoXY(10, 53);
			sprintf(MSG, "TR:%.2d - TO:%.2d",(int) (cont_tempo_troca_ar/60),(int)(cont_tempo_fan_on));

			SSD1306_Puts(MSG, &Font_7x10, 1);

		} else {
			SSD1306_GotoXY(1, 3);
			sprintf(MSG, " Time: (%.2d:%.2d:%.2d)", sTime.Hours, sTime.Minutes,
					sTime.Seconds); //C%.2f, TempC_Trab);
			SSD1306_Puts(MSG, &Font_7x10, 1);
			DHT_GetData(&DHT11_Data);

			TempC = DHT11_Data.Temperature;
			Humidity = DHT11_Data.Humidity;

			SSD1306_GotoXY(5, 25);
			sprintf(MSG, "T(%.2u) H(%.2u)", (int) TempC, (int) Humidity);
			SSD1306_Puts(MSG, &Font_11x18, 1);

			htim2.Instance->CCR1 = 0;
			htim2.Instance->CCR2 = 0;
			htim2.Instance->CCR3 = 0;
			htim2.Instance->CCR4 = 0;


		}
		SSD1306_UpdateScreen();
		HAL_Delay(2000);
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
