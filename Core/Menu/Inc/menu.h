/*
 * menu.h
 *
 *  Created on: Oct 15, 2022
 *      Author: vagne
 */

#ifndef MENU_INC_MENU_H_
#define MENU_INC_MENU_H_
#include "stm32L4xx_hal.h"
#include<stdio.h>
#include <stdio.h> /* Standard input/output definitions */
#include <stdint.h>
#include <string.h> /* String function definitions */
//

typedef struct
	{
		uint8_t MATERIAL[5];
		uint16_t TEMP;
		uint16_t TEMPO;
	} STRUCT_MAT;

//extern STRUCT_MAT Filament;

const static STRUCT_MAT Filament[18] =
	{
		{ " ABS ", 65, 420 }, //00
		    { " PLA ", 50, 300 }, //01
		    { "PETG ", 65, 420 }, //02
		    { "TPU  ", 65, 360 }, //03
		    { "TPC  ", 68, 360 }, //04
		    { "TPE  ", 58, 360 }, //05
		    { " ASA ", 60, 300 }, //06
		    { "HIPS ", 60, 300 }, //07
		    { " PA  ", 80, 1440 }, //08
		    { " PC  ", 70, 480 }, //09
		    { "PEEK ", 70, 540 }, //10
		    { "PMMA ", 65, 420 }, //11
		    { "POM  ", 65, 540 }, //12
		    { " PP  ", 55, 360 }, //13
		    { " PS  ", 65, 360 }, //14
		    { " PVA ", 48, 600 }, //15
		    { "SAN  ", 62, 300 }, //16
		    { "100", 100, 420 } //17
	};
extern uint8_t menu ( uint8_t *perfil );
extern void set_material ( void );
extern uint8_t key_press ( void );
#endif /* MENU_INC_MENU_H_ */
