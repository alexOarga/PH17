/*********************************************************************************************
 * File£º	main.c
 * Author:	embest
 * Desc£º	c main entry
 * History:
 *********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"
#include "tp.h"
#include "def.h"
#include "lcd.h"
#include "timer.h"
#include "timer2.h"
#include "8led.h"
#include "excepcion.h"
#include "debugPila.h"
#include "button.h"

#define CHAR_VER 16
#define CHAR_HOR 8

void calibrar() {

	int detectar_pulsacion;
	int calibraciones_validas = 0;
	int calibraciones[4] = { 0, 0, 0, 0 };
	INT8U* pucChar1 = (INT8U*) "Pulsar los numeros";
	INT8U* pulsaciones1 = (INT8U*) "1";
	INT8U* pulsaciones2 = (INT8U*) "2";
	INT8U* pulsaciones3 = (INT8U*) "3";
	INT8U* pulsaciones4 = (INT8U*) "4";

	Lcd_Clr();
	Lcd_Active_Clr();
	while (calibraciones_validas !=1) {

		Lcd_DspAscII8x16(LCD_XSIZE / 2, LCD_YSIZE / 2, BLACK, pucChar1);
		Lcd_Dma_Trans();
		Delay(3000);
		Lcd_Clr();
		Lcd_Active_Clr();

		Lcd_DspAscII8x16(LCD_XSIZE / 2, LCD_YSIZE / 2, BLACK, pucChar1);
		Lcd_DspAscII8x16(LCD_XSIZE / 2, 0, BLACK, pulsaciones1);
		Lcd_Dma_Trans();

		// ultimo contador de pulsaciones
		detectar_pulsacion = ultima_pulsacion();
		// mientras no varia el numero de pulsaciones
		while (ultima_pulsacion() == detectar_pulsacion) {
		}
		// aqui ya ha pulsado
		Lcd_Clr();
		Lcd_Active_Clr();
		if (calibraciones[0] < 2) {
			LIM_ARRIBA_X += pulsacion_X_CORD();
			LIM_ARRIBA_Y += pulsacion_Y_CORD();
			if (LIM_ARRIBA_X <= 0 || LIM_ARRIBA_Y <= 0) {
				LIM_ARRIBA_X -= pulsacion_X_CORD();
				LIM_ARRIBA_Y -= pulsacion_Y_CORD();
			} else
				calibraciones[0]++;
		}

		detectar_pulsacion = ultima_pulsacion();
		Lcd_DspAscII8x16(LCD_XSIZE / 2, LCD_YSIZE / 2, BLACK, pucChar1);
		Lcd_DspAscII8x16(0, LCD_YSIZE / 2, BLACK, pulsaciones2);
		Lcd_Dma_Trans();

		int anterior = pulsacion_X_CORD();
		while (ultima_pulsacion() == detectar_pulsacion) {
		}
		Lcd_Clr();
		Lcd_Active_Clr();
		if (calibraciones[1] < 2) {
			LIM_IZQ_X += pulsacion_X_CORD();
			LIM_IZQ_Y += pulsacion_Y_CORD();
			if (LIM_IZQ_X <= 0 || LIM_IZQ_Y <= 0
					||  pulsacion_X_CORD()>anterior) {
				LIM_IZQ_X -= pulsacion_X_CORD();
				LIM_IZQ_Y -= pulsacion_Y_CORD();
			} else
				calibraciones[1]++;
		}
		detectar_pulsacion = ultima_pulsacion();
		Lcd_DspAscII8x16(LCD_XSIZE / 2, LCD_YSIZE / 2, BLACK, pucChar1);
		Lcd_DspAscII8x16(LCD_XSIZE / 2, LCD_YSIZE - CHAR_VER, BLACK,
				pulsaciones3);
		Lcd_Dma_Trans();

		anterior = pulsacion_Y_CORD();
		while (ultima_pulsacion() == detectar_pulsacion) {
		}
		Lcd_Clr();
		Lcd_Active_Clr();
		if (calibraciones[2] < 2) {
			LIM_ABAJO_X += pulsacion_X_CORD();
			LIM_ABAJO_Y += pulsacion_Y_CORD();
			if (LIM_ABAJO_X <= 0 || LIM_ABAJO_Y <= 0
					|| pulsacion_Y_CORD() > anterior) {
				LIM_ABAJO_X -= pulsacion_X_CORD();
				LIM_ABAJO_Y -= pulsacion_Y_CORD();
			} else
				calibraciones[2]++;
		}

		detectar_pulsacion = ultima_pulsacion();
		Lcd_DspAscII8x16(LCD_XSIZE / 2, LCD_YSIZE / 2, BLACK, pucChar1);
		Lcd_DspAscII8x16(LCD_XSIZE - CHAR_VER, LCD_YSIZE / 2, BLACK,
				pulsaciones4);
		Lcd_Dma_Trans();

		anterior = pulsacion_X_CORD();
		while (ultima_pulsacion() == detectar_pulsacion) {
		}
		Lcd_Clr();
		Lcd_Active_Clr();
		if (calibraciones[3] < 2) {
			LIM_DCH_X += pulsacion_X_CORD();
			LIM_DCH_Y += pulsacion_Y_CORD();
			if (LIM_DCH_X <= 0 || LIM_DCH_Y <= 0
					|| pulsacion_X_CORD()< anterior  ) {
				LIM_DCH_X -= pulsacion_X_CORD();
				LIM_DCH_Y -= pulsacion_Y_CORD();
			} else
				calibraciones[3]++;
		}

		if (calibraciones[0] >= 2 && calibraciones[1] >= 2
				&& calibraciones[2] >= 2 && calibraciones[3] >= 2) {
			calibraciones_validas = 1;
		}
	}

	Delay(6000);

	Xmin = LIM_IZQ_X / calibraciones[1];
	Xmax = LIM_DCH_X / calibraciones[3];
	Ymin = LIM_ABAJO_Y / calibraciones[2];
	Ymax = LIM_ARRIBA_Y / calibraciones[0];
}

/*--- initial function ---*/

void inicializar_sistema() {

	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	Eint4567_init();// Inicializacion los pulsadores. Cada vez que se pulse se ver reflejado en el 8led
	D8Led_init();       // Inicializacion el 8led
	debug_init();		// Inicializacion la pila de debug
	init_exception();	// Inicializacion el tratamiento de excepciones
	Lcd_Init();			// Inicializacion del LCD
	TS_init();			// Inicializacion de la TS
	timer_init();	    // Inicializacion del temporizador
	timer2_inicializar();	// Inicializacion del timer2

	/*
	 * Calibración necesaria para el reconocimiento de la pantalla
	 */

	calibrar();
}

/*--- function code ---*/
/*********************************************************************************************
 * name:		main
 * func:		c code entry
 * para:		none
 * ret:		none
 * modify:
 * comment:
 *********************************************************************************************/
void Main(void) {
	/*
	 * Inicializacion necesaria para el funcionamiento del juego
	 */

	inicializar_sistema();

	/*
	 * Comienzo del juego
	 */

	reversi8();
	TS_close();
}
