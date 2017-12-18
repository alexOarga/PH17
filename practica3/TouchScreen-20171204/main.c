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

/////////////////////////////////////////////////////////////////////
// limites de la pantalla tras calibrar
int LIM_ARRIBA = 0;
int LIM_ABAJO = 0;
int LIM_IZQ = 0;
int LIM_DCH = 0;

	char yn;
/*--- function declare ---*/
void Main(void);

/*--- extern function ---*/
extern void Lcd_Test();

void calibrar(){
	int detectar_pulsacion;

	Lcd_DisplayString(LCD_XSIZE/2, LCD_YSIZE/2, "Pulsar '+'");
	// ultimo contador de pulsaciones
	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(LCD_XSIZE/2, LCD_YSIZE, '+');
	// mientras no varia el numero de pulsaciones
	while(ultima_pulsacion()==detectar_pulsacion){}
	// aqui ya ha pulsado
	LIM_ARRIBA = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(0, LCD_YSIZE/2, '+');
	while(ultima_pulsacion()==detectar_pulsacion){}
	LIM_IZQ = pulsacion_X_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(LCD_XSIZE/2, 0, '+');
	while(ultima_pulsacion()==detectar_pulsacion){}
	LIM_ABAJO = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(LCD_XSIZE, LCD_YSIZE/2, '+');
	while(ultima_pulsacion()==detectar_pulsacion){}
	LIM_DCH = pulsacion_X_CORD();
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
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    /******************/
	/* user interface */
	/******************/
	//Lcd_Test();
	//TS_Test();
	TS_init();
	calibrar();	

	while(1)
	 {
	   

	   //if(yn == 0x52) TS_Test();// R to reset the XY REC
	   //else break;
	 }
	
	TS_close();
}
