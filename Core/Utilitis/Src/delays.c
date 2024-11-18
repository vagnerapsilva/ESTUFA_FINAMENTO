/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *        TECNODEV TECNOLOGIA DE DESENVOLVIMENTIO DE SOFTWARE E SERVICOS       *
 *        TEL: (011) 4109-0577                 SITE: www.tecnodev.com.br       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Includes ------------------------------------------------------------------*/
#include "stm32L4xx_hal.h"
#include "delays.h"
#include "tim.h"

/* Includes platform independent ----------------------------------------*/
uint32_t Delay_us;
uint8_t sleep_cont;
uint8_t menu_tempo = 0;
uint32_t cont_tempo_troca_ar = 0; //segundo
uint8_t intervalo_troca_ar = 90; //minutos
uint8_t cont_tempo_fan_on = 0;
uint8_t tempo_fan_on = 20; //em segundos
uint8_t em_processo = 0;

extern uint8_t key;
extern uint8_t fan2;
extern float TempC;
extern float TempC_Trab;
extern float Humidity;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  SYSTICK callback.
 * @retval None
 */
void HAL_SYSTICK_Callback(void) {
	static uint32_t cont_tempo = 0;

	if (cont_tempo > 1000) //a cada 1 segundo
			{
		cont_tempo = 0;
		sleep_cont++;

		if (sleep_cont > 10) {
			if (key > 0)
				key = 0;
			menu_tempo = 1;
			sleep_cont = 0;
		}
		if (em_processo == 1) {
			if (TempC > (TempC_Trab - 5)) {
				cont_tempo_troca_ar++;

				if(fan2==100) cont_tempo_fan_on++;
				if (cont_tempo_troca_ar > (intervalo_troca_ar * 60)) {
					cont_tempo_troca_ar = 0;
					fan2 = 100;
					cont_tempo_fan_on = 0;
					intervalo_troca_ar=60;
				}
				else
				{
					fan2 = 15;
				}
				if (cont_tempo_fan_on > (tempo_fan_on)) {
					cont_tempo_fan_on = 0;
					fan2 = 15;
				}
			} else {
				cont_tempo_fan_on = 0;
				fan2 = 0;
				cont_tempo_troca_ar = 0;
			}
		}
	}

	cont_tempo++;
}

void DelayUs(uint32_t xDelay) {
	__HAL_TIM_SET_COUNTER(&htim17, 0);
	while (__HAL_TIM_GET_COUNTER(&htim17) < (xDelay / 2))
		;

}

void DelayMs(uint32_t xDelay) {
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = xDelay;

	/* Add a freq to guarantee minimum wait */
	if (wait < HAL_MAX_DELAY) {
		wait += (uint32_t) (uwTickFreq);
	}

	while ((HAL_GetTick() - tickstart) < wait) {
	}
}

void DelayS(uint32_t xDelay) {
	while (xDelay--) {
		DelayMs(1000);
	};
}

/**
 * @brief  Inserts a delay time.
 * @param  nCount: specifies the delay time length.
 * @retval None
 */
void Delay( __IO uint32_t nCount) {
	__IO uint32_t index = 0;
	for (index = (0xFF * nCount); index != 0; index--) {
	}
}

