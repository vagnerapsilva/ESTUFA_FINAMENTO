/*
 * temperatura.h
 *
 *  Created on: Nov 18, 2020
 *      Author: vagner
 */

#ifndef UTILITIS_INC_TEMPERATURA_H_
#define UTILITIS_INC_TEMPERATURA_H_
/* Choose PID parameters */
#define PID_PARAM_KP        100            /* Proporcional */
#define PID_PARAM_KI        0.025        /* Integral */
#define PID_PARAM_KD        20            /* Derivative */

extern void Temperatura(void);
extern void Temperatura_Init(void);


#endif /* UTILITIS_INC_TEMPERATURA_H_ */
