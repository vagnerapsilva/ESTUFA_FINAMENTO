/************** MAKE CHANGES HERE ********************/
#include "stm32l4xx_hal.h"
#include "delays.h"

//#define TYPE_DHT11    // define according to your sensor
#define TYPE_DHT22

#define DHT_PORT GPIOA
#define DHT_PIN GPIO_PIN_4

/*******************************************     NO CHANGES AFTER THIS LINE      ****************************************************/
uint8_t RH1, RH2, TC1, TC2, SUM, CHECK;
uint32_t pMillis, cMillis;
#include "DHT.h"
//
//uint32_t DWT_Delay_Init(void)
//{
//  /* Disable TRC */
//  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
//  /* Enable TRC */
//  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
//
//  /* Disable clock cycle counter */
//  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
//  /* Enable  clock cycle counter */
//  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
//
//  /* Reset the clock cycle counter value */
//  DWT->CYCCNT = 0;
//
//     /* 3 NO OPERATION instructions */
//     __ASM volatile ("NOP");
//     __ASM volatile ("NOP");
//  __ASM volatile ("NOP");
//
//  /* Check if clock cycle counter has started */
//     if(DWT->CYCCNT)
//     {
//       return 0; /*clock cycle counter started*/
//     }
//     else
//  {
//    return 1; /*clock cycle counter not started*/
//  }
//}
//
//__STATIC_INLINE void DelayUs(volatile uint32_t microseconds)
//{
//  uint32_t clk_cycle_start = DWT->CYCCNT;
//
//  /* Go to number of cycles for system */
//  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
//
//  /* Delay till end */
//  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
//}

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
	{
		GPIO_InitTypeDef GPIO_InitStruct =
			{ 0 };
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; //GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
	{
		GPIO_InitTypeDef GPIO_InitStruct =
			{ 0 };
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL; //GPIO_PULLDOWN;//GPIO_PULLUP ;//GPIO_NOPULL;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}
/*

 void DHT_Start(void)
 {
 //DWT_Delay_Init();
 3 NO OPERATION instructions
 __ASM volatile ("NOP");
 __ASM volatile ("NOP");
 __ASM volatile ("NOP");
 Set_Pin_Output(DHT_PORT, DHT_PIN);  // set the pin as output
 HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 0);   // pull the pin low

 #if defined(TYPE_DHT11)
 DelayMs(18);   // wait for 18ms
 #endif

 #if defined(TYPE_DHT22)
 DelayMs(1);  // >1ms DelayUs
 #endif

 HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 1);   // pull the pin high
 DelayUs(20);   // wait for 30us
 Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input
 }

 uint8_t DHT_Check_Response(void)
 {
 uint8_t Response = 0;
 DelayUs(40);
 if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
 {
 DelayUs(80);
 if ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
 Response = 1;
 }

 uint32_t pMillis = HAL_GetTick();
 uint32_t cMillis = HAL_GetTick();
 while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && pMillis + 2 > cMillis)
 {
 cMillis = HAL_GetTick();
 }

 uint32_t tickstart = HAL_GetTick();
 while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN))&&)   // wait for the pin to go low
 {
 if ((HAL_GetTick() - tickstart) > 100) {
 return 0xAA;
 }
 return Response;
 }

 uint8_t DHT_Read(void)
 {
 uint8_t i, j;
 for (j = 0; j < 8; j++)
 {
 while (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
 ;   // wait for the pin to go high
 DelayUs(40);   // wait for 40 us
 if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))) // if the pin is low
 {
 i &= ~(1 << (7 - j));   // write 0
 } else
 i |= (1 << (7 - j));  // if the pin is high, write 1
 while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
 ;  // wait for the pin to go low
 }
 return i;
 }
 */

uint8_t DHT22_Start(void)
	{
		uint8_t Response = 0;
		Set_Pin_Output(DHT_PORT, DHT_PIN);  // set the pin as output
		HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 0);   // pull the pin low
		DelayUs(1300);   // wait for 1300us
		HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, 1);   // pull the pin high
		DelayUs(30);   // wait for 30us

		Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input
		DelayUs(40);
		if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
			{
				DelayUs(80);
				if ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)))
					Response = 1;
			}
		pMillis = HAL_GetTick();
		cMillis = HAL_GetTick();
		while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) && pMillis + 2 > cMillis)
			{
				cMillis = HAL_GetTick();
			}
		return Response;
	}

uint8_t DHT22_Read(void)
	{
		uint8_t a, b;
		for (a = 0; a < 8; a++)
			{
				pMillis = HAL_GetTick();
				cMillis = HAL_GetTick();
				while (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
						&& pMillis + 2 > cMillis)
					{  // wait for the pin to go high
						cMillis = HAL_GetTick();
					}
				DelayUs(40);   // wait for 40 us
				if (!(HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))) // if the pin is low
					b &= ~(1 << (7 - a));
				else
					b |= (1 << (7 - a));
				pMillis = HAL_GetTick();
				cMillis = HAL_GetTick();
				while ((HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
						&& pMillis + 2 > cMillis)
					{  // wait for the pin to go low
						cMillis = HAL_GetTick();
					}
			}
		return b;
	}

void DHT_GetData(DHT_DataTypedef *DHT_Data)
	{
		if (DHT22_Start())
			{
				RH1 = DHT22_Read(); // First 8bits of humidity
				RH2 = DHT22_Read(); // Second 8bits of Relative humidity
				TC1 = DHT22_Read(); // First 8bits of Celsius
				TC2 = DHT22_Read(); // Second 8bits of Celsius
				SUM = DHT22_Read(); // Check sum
				CHECK = RH1 + RH2 + TC1 + TC2;
				if (CHECK == SUM)
					{
						if (TC1 > 127) // If TC1=10000000, negative temperature
							{
								DHT_Data->Temperature = (float) TC2 / 10 * (-1);
							} else
							{
								DHT_Data->Temperature = (float) ((TC1 << 8)
										| TC2) / 10;
							}
						//tFahrenheit = tCelsius * 9/5 + 32;
						DHT_Data->Humidity = (float) ((RH1 << 8) | RH2) / 10;
					}
			}
	}
/*
 {
 DHT_Start();
 if (DHT_Check_Response() == 1)
 {
 Rh_byte1 = DHT_Read();
 Rh_byte2 = DHT_Read();
 Temp_byte1 = DHT_Read();
 Temp_byte2 = DHT_Read();
 SUM = DHT_Read();

 if (SUM == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2))
 {
 #if defined(TYPE_DHT11)
 DHT_Data->Temperature = Temp_byte1;
 DHT_Data->Humidity = Rh_byte1;
 #endif

 #if defined(TYPE_DHT22)
 DHT_Data->Temperature =
 ((Temp_byte1 << 8) | Temp_byte2);
 DHT_Data->Humidity = ((Rh_byte1 << 8) | Rh_byte2);
 #endif
 }
 }
 }
 */

