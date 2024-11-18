/**
 * @file delays.h
 *
 * Timer-related support functions, needed by the ESS/TFM comm library
 *
 * Copyright (C) 2001-2006 UPEK Inc.
 */

#ifndef __DELAYS_H__
#define __DELAYS_H__
#include "stm32L4xx_hal.h"
//#include "stm322xg_eval.h"


//========================================================================
//		Exported functions
//========================================================================
extern uint32_t Delay_us;
extern void DelayUs(uint32_t xDelay);
extern void DelayMs(uint32_t xDelay);
extern void DelayS(uint32_t xDelay);
extern void Delay(__IO uint32_t nCount);


#endif // __DELAYS_H__



