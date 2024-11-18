/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *        TECNODEV TECNOLOGIA DE DESENVOLVIMENTIO DE SOFTWARE E SERVICOS       *
 *        TEL: (011) 4109-0577                 SITE: www.tecnodev.com.br       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Includes ------------------------------------------------------------------*/
#include "stm32L4xx_hal.h"
#include "temperatura.h"
#include "tim.h"
#include "arm_math.h"
#include "arm_common_tables.h"
#include "arm_const_structs.h"
#include "math.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variaveis do arquivo
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ARM PID Instance, float_32 format */
arm_pid_instance_f32 PIDS;
arm_pid_instance_f32 PIDI;
/* PID error */
float pid_error_s;
float pid_error_i;
/* Duty cycle for PWM */
float duty_s = 0;
float duty_i = 0;
float fan_s=0;

extern float TempC;
extern float TempC_Trab;
extern float Humidity;

extern uint8_t fan1;
extern uint8_t fan2;
extern uint8_t fan3;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Inicializa valores para temperatura e PID
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Temperatura_Init(void)
{
	/* Set PID parameters Superior*/
	/* Set this for your needs */
//	PIDS.Kp = 13.8; //PID_PARAM_KP;		/* Proporcional */
//	PIDS.Ki = 0.060; //PID_PARAM_KI;		/* Integral */
//	PIDS.Kd = 30; //PID_PARAM_KD;		/* Derivative */

	/* Set this for your needs */
		// PIDS.Kp = 100; //PID_PARAM_KP;		/* Proporcional */
		// PIDS.Ki = 0.025; //PID_PARAM_KI;		/* Integral */
		// PIDS.Kd = 20; //PID_PARAM_KD;		/* Derivative */

	/* Set this for your needs */
//	PIDS.Kp = 100.00; //PID_PARAM_KP;		/* Proporcional */
//	PIDS.Ki = 0.025; //PID_PARAM_KI;		/* Integral */
//	PIDS.Kd = 30.00; //PID_PARAM_KD;		/* Derivative */

	/* Set this for your needs */
	  PIDS.Kp = 10.00;//PID_PARAM_KP;		/* Proporcional */
	  PIDS.Ki = 0.023;//PID_PARAM_KI;		/* Integral */
	  PIDS.Kd = 305.4;//PID_PARAM_KD;		/* Derivative */

	/* Set this for your needs */
//	PIDS.Kp = 97.10; //PID_PARAM_KP;		/* Proporcional */
//	PIDS.Ki = 1.410; //PID_PARAM_KI;		/* Integral */
//	PIDS.Kd = 1675.16; //PID_PARAM_KD;		/* Derivative */

	/* Initialize PID system, float32_t format */
	arm_pid_init_f32(&PIDS, 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Controla a temperatura
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Temperatura(void)
{
	//if (TempC > (TempC_Trab-5))
	//{
	/* Calculate error */
	pid_error_s = TempC_Trab - TempC;

	/* Calculate PID here, argument is error */
	/* Output data will be returned, we will use it as duty cycle parameter */
	duty_s = arm_pid_f32(&PIDS, pid_error_s);

	/* Check overflow, duty cycle in percent */
	if (duty_s > 100) {
		duty_s = 100;
	} else if (duty_s < 0) {
		duty_s = 0;
	}


	if (TempC > (TempC_Trab + 5)) //seguranca contra falha
	{
		duty_s = 0;
	}

	htim2.Instance->CCR1 = ((htim2.Init.Period * (duty_s * 0.45)) / 100);
	htim2.Instance->CCR4 = ((htim2.Init.Period*fan3)/100);
	htim2.Instance->CCR3 = ((htim2.Init.Period*fan2)/100);
	htim2.Instance->CCR2= ((htim2.Init.Period*fan1)/100);
	if(TempC > (TempC_Trab*0.85))
	{
		fan3=100;
	}
	else
	{
		fan3=0;
	}
	//	} else {
	//		htim2.Instance->CCR1 = 0;
	//		//liga fan2 100%
	//		htim2.Instance->CCR4 = 2000;
	//		htim2.Instance->CCR3 =  (2000 * 0.99);
	//	}
}
