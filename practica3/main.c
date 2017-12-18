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
/*--- extern function ---*/



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

	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se ver reflejado en el 8led
	D8Led_init();       // inicializamos el 8led
	debug_init();
	init_exception();
	timer2_inicializar();
    Lcd_Init();
    TS_init();

	/*
	 * Calibración necesaria para el reconocimiento de la pantalla
	 */

	calibrar();	

	/*
	 * Comienzo del juego
	 */

	reversi8();
	
	TS_close();
}


void calibrar(){
	int detectar_pulsacion;
	volatile INT8U* pucChar1 = "Pulsar los numeros";
	volatile INT8U* pulsaciones1 = "1";
	volatile INT8U* pulsaciones2 = "2";
	volatile INT8U* pulsaciones3 = "3";
	volatile INT8U* pulsaciones4 = "4";

	Lcd_Clr();
	Lcd_Active_Clr();

	Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
	Lcd_Dma_Trans();
	Delay(3000);
	Lcd_Clr();
	Lcd_Active_Clr();

	Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
	Lcd_DspAscII8x16(LCD_XSIZE/2, 0,BLACK,pulsaciones1);
	Lcd_Dma_Trans();
	Delay(500);

	//Lcd_DspAscII8x6(LCD_XSIZE, LCD_YSIZE, BLACK, pucChar1);
	// ultimo contador de pulsaciones
	detectar_pulsacion = ultima_pulsacion();
	//Lcd_DspAscII8x16(LCD_XSIZE/2, 0,BLACK,mas1);
	// mientras no varia el numero de pulsaciones
	while(ultima_pulsacion()==detectar_pulsacion){}
	// aqui ya ha pulsado
	Lcd_Clr();
	Lcd_Active_Clr();
	LIM_ARRIBA_X = pulsacion_X_CORD();
	LIM_ARRIBA_Y = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
	Lcd_DspAscII8x16(0, LCD_YSIZE/2,BLACK,pulsaciones2);
	Lcd_Dma_Trans();
	Delay(500);
	while(ultima_pulsacion()==detectar_pulsacion){}
	Lcd_Clr();
	Lcd_Active_Clr();
	LIM_IZQ_X = pulsacion_X_CORD();
	LIM_IZQ_Y = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
	Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE-CHAR_VER,BLACK,pulsaciones3);
	Lcd_Dma_Trans();
	Delay(500);
	while(ultima_pulsacion()==detectar_pulsacion){}
	Lcd_Clr();
	Lcd_Active_Clr();
	LIM_ABAJO_X = pulsacion_X_CORD();
	LIM_ABAJO_Y = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DspAscII8x16(LCD_XSIZE/2, LCD_YSIZE/2,BLACK,pucChar1);
	Lcd_DspAscII8x16(LCD_XSIZE-CHAR_VER, LCD_YSIZE/2,BLACK,pulsaciones4);
	Lcd_Dma_Trans();
	Delay(500);
	while(ultima_pulsacion()==detectar_pulsacion){}
	Lcd_Clr();
	Lcd_Active_Clr();
	LIM_DCH_X = pulsacion_X_CORD();
	LIM_DCH_Y = pulsacion_Y_CORD();

	Delay(6000);

	Xmin = LIM_IZQ_X;
	Xmax = LIM_DCH_X;
	Ymin=  LIM_ABAJO_Y;
	Ymax=  LIM_ARRIBA_Y;
}

