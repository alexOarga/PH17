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

#define CHAR_VER 16
#define CHAR_HOR 8

char yn;

/*--- initial function ---*/

void inicializar_sistema(){
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// Inicializacion los pulsadores. Cada vez que se pulse se ver reflejado en el 8led
	D8Led_init();       // Inicializacion el 8led
	debug_init();		// Inicializacion la pila de debug
	init_exception();	// Inicializacion el tratamiento de excepciones
	timer2_inicializar();	// Inicializacion del timer2
    Lcd_Init();			// Inicializacion del LCD
    TS_init();			// Inicializacion de la TS

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
void Main(void)
{
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


void calibrar(){

	int detectar_pulsacion;
	int calibraciones_validas=0;
	int calibraciones[4]={0,0,0,0};
	volatile INT8U* pucChar1 = "Pulsar los numeros";
	volatile INT8U* pulsaciones1 = "1";
	volatile INT8U* pulsaciones2 = "2";
	volatile INT8U* pulsaciones3 = "3";
	volatile INT8U* pulsaciones4 = "4";

	Lcd_Clr();
	Lcd_Active_Clr();
	while(calibraciones_validas<2){
		int check=4;

		Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
		Lcd_Dma_Trans();
		Delay(3000);
		Lcd_Clr();
		Lcd_Active_Clr();

		Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
		Lcd_DspAscII8x16(LCD_XSIZE/2, 0,BLACK,pulsaciones1);
		Lcd_Dma_Trans();

		// ultimo contador de pulsaciones
		detectar_pulsacion = ultima_pulsacion();
		// mientras no varia el numero de pulsaciones
		while(ultima_pulsacion()==detectar_pulsacion){}
		// aqui ya ha pulsado
		Lcd_Clr();
		Lcd_Active_Clr();
		LIM_ARRIBA_X += pulsacion_X_CORD();
		LIM_ARRIBA_Y += pulsacion_Y_CORD();
		if(LIM_ARRIBA_X<=0 || LIM_ARRIBA_Y<=0) check--;
		else calibraciones[0]++;

		detectar_pulsacion = ultima_pulsacion();
		Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
		Lcd_DspAscII8x16(0, LCD_YSIZE/2,BLACK,pulsaciones2);
		Lcd_Dma_Trans();

		while(ultima_pulsacion()==detectar_pulsacion){}
		Lcd_Clr();
		Lcd_Active_Clr();
		LIM_IZQ_X += pulsacion_X_CORD();
		LIM_IZQ_Y += pulsacion_Y_CORD();
		if(LIM_IZQ_X<=0 || LIM_IZQ_Y<=0) check--;
		else calibraciones[1]++;

		detectar_pulsacion = ultima_pulsacion();
		Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
		Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE-CHAR_VER,BLACK,pulsaciones3);
		Lcd_Dma_Trans();

		while(ultima_pulsacion()==detectar_pulsacion){}
		Lcd_Clr();
		Lcd_Active_Clr();
		LIM_ABAJO_X += pulsacion_X_CORD();
		LIM_ABAJO_Y += pulsacion_Y_CORD();
		if(LIM_ABAJO_X<=0 || LIM_ABAJO_Y<=0) check--;
		else calibraciones[2]++;

		detectar_pulsacion = ultima_pulsacion();
		Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
		Lcd_DspAscII8x16(LCD_XSIZE-CHAR_VER, LCD_YSIZE/2,BLACK,pulsaciones4);
		Lcd_Dma_Trans();

		while(ultima_pulsacion()==detectar_pulsacion){}
		Lcd_Clr();
		Lcd_Active_Clr();
		LIM_DCH_X += pulsacion_X_CORD();
		LIM_DCH_Y += pulsacion_Y_CORD();
		if(LIM_DCH_X<=0 || LIM_DCH_Y<=0) check--;
		else calibraciones[3]++;

		if(check==4) calibraciones_validas++;
	}

	Delay(6000);

	Xmin = LIM_IZQ_X/calibraciones[1];
	Xmax = LIM_DCH_X/calibraciones[3];
	Ymin=  LIM_ABAJO_Y/calibraciones[2];
	Ymax=  LIM_ARRIBA_Y/calibraciones[0];
}

