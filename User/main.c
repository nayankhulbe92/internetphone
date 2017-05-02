/**
 *	Keil project for LIS302DL or LIS3DSH accelerometer on STM32F4-Discovery board
 *
 *  Designed to work with STM32F4-Discovery board with onboard device
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 * On init, if you have old LIS302DL on STM32F4-Discovery board,
 * LED_GREEN and LED_RED will be turned on for 2 seconds,
 * if you have new LIS3DSH on STM32F4-Discovery board,
 * LED_BLUE and LED_ORANGE will be turned on for 2 seconds.
 *
 * If all leds are on, there was an error somewhere.
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
#include "tm_stm32f4_servo.h"
/* Accelerometer data structure */
TM_LIS302DL_LIS3DSH_t Axes_Data;
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	
	/* Detect proper device */
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Turn on GREEN and RED */
		TM_DISCO_LedOn(LED_GREEN | LED_RED);
		/* Initialize LIS302DL */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Turn on BLUE and ORANGE */
		TM_DISCO_LedOn(LED_BLUE | LED_ORANGE);
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	} else {
		/* Device is not recognized */
		
		/* Turn on ALL leds */
		TM_DISCO_LedOn(LED_GREEN | LED_RED | LED_BLUE | LED_ORANGE);
		
		/* Infinite loop */
		while (1);
	}
	
	/* Delay for 2 seconds */
	//Delayms(2000);
	TM_SERVO_t Servo1, Servo2;
	TM_SERVO_Init(&Servo1, TIM2, TM_PWM_Channel_3, TM_PWM_PinsPack_2);
	
	/* Initialize servo 2, TIM2, Channel 2, Pinspack 2 = PB3 */
	TM_SERVO_Init(&Servo2, TIM2, TM_PWM_Channel_2, TM_PWM_PinsPack_2);
	//TM_SERVO_SetDegrees(&Servo1, 0);
	

	TM_SERVO_SetDegrees(&Servo1, 110);
	TM_SERVO_SetDegrees(&Servo2, 0);
	Delay(10000);
	TM_SERVO_SetDegrees(&Servo2, 100);
	Delay(1000);
	while (1) {
		/* Read axes data from initialized accelerometer */
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
		int x=map(Axes_Data.X,-1024,1024,90,114);
		TM_SERVO_SetDegrees(&Servo2, x);

		int y=map(Axes_Data.Y,-1024,1024,70,110);
		TM_SERVO_SetDegrees(&Servo1, y);

	}
}
