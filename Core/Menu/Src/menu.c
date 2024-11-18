/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *        TECNODEV TECNOLOGIA DE DESENVOLVIMENTIO DE SOFTWARE E SERVICOS       *
 *        TEL: (011) 4109-0577                 SITE: www.tecnodev.com.br       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Includes ------------------------------------------------------------------*/

#include "stm32L4xx_hal.h"
#include "rtc.h"
#include "main.h"
#include "menu.h"

#include "DHT.h"
#include "delays.h"
//#include "gifs.h"
#include "ssd1306.h"
#include "fonts.h"
//#include "bitmaps.h"
#include "temperatura.h"

/* Includes platform independent ----------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t key = 0;
extern uint8_t menu_tempo;
extern uint8_t sleep_cont;

//const static STRUCT_MAT Filament =
//		{
//		 " ABS ","Baixo custo, alta resistencia, usinabilidade",65,420
//		};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void set_material(void) {
//	strcpy(Filament.MATERIAL,"ABS");
//	strcpy(Filament.DESCRICAO,"Baixo custo, alta resistencia, usinabilidade");
//	Filament.TEMP=65;
//	Filament.TEMPO=420;
}

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* Prevent unused argument(s) compilation warning */
	if (GPIO_Pin == sw_01_Pin) {
		key = 1;
	}

	/* NOTE: This function should not be modified, when the callback is needed,
	 the HAL_GPIO_EXTI_Callback could be implemented in the user file
	 */
}

//-------------------------------------------------------------------------------------------
// key
//-------------------------------------------------------------------------------------------
uint8_t key_press(void) {
	if (key == 1) {
		//__HAL_GPIO_EXTI_CLEAR_IT(sw_01_Pin);
		key = 0;
		DelayMs(250);
		if (HAL_GPIO_ReadPin(sw_01_GPIO_Port, sw_01_Pin) == 0)
			return 0;
	}
	return 1;
}

//-------------------------------------------------------------------------------------------
// select
//-------------------------------------------------------------------------------------------
uint8_t get_menu(uint8_t *count, uint8_t limite) {
	uint8_t select = 0;
	uint8_t key_ok = 0;
	uint8_t count_temp = *count;

	TIM3->CNT = 0;
	do {
		select = (((TIM3->CNT) >> 2) & 0xFF);
		if (key_press() == 0) {
			key_ok = 1;
			break;
		}
		if (menu_tempo == 1) return 0;
		DelayMs(300);
	} while (select == 0);
	sleep_cont=0;
	menu_tempo = 0;
	if (key_ok == 1)
		return 1;
	if ((select > 0) && (select < 10))
		if (count_temp > limite)
			count_temp = 0;
		else
			count_temp++;
	else {
		if (count_temp == 0)
			count_temp = limite;
		else
			count_temp--;
	}
	*count = count_temp;
	return 0;
}
//-------------------------------------------------------------------------------------------
// Menu principal
//-------------------------------------------------------------------------------------------
uint8_t menu(uint8_t *perfil) {
	char MSG[50];
	uint8_t key_press = 0;
	uint8_t count = *perfil;
	key = 0;
	sleep_cont = 0;
	menu_tempo = 0;

	SSD1306_Clear();
	SSD1306_GotoXY(1, 3);
	sprintf(MSG, "Perfis de Secagem");
	SSD1306_Puts(MSG, &Font_7x10, 1);

	do {
		memset(MSG, 0, sizeof(MSG));
		SSD1306_GotoXY(15, 29);
		strcpy(MSG, (char*) Filament[count].MATERIAL);
		SSD1306_Puts(MSG, &Font_16x26, 1);
		SSD1306_UpdateScreen();
		key_press = get_menu(&count, 17);
		if (key_press == 1) {
			key = 0;
			MX_RTC_Init();
			break;
		}
		if (menu_tempo == 1)
		{
			SSD1306_Clear();
			return 0;
		}
	} while (1);
	*perfil = count;

	SSD1306_Clear();
	return key_press;
}

