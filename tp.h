/*********************************************************************************************
* File£º	tp.H
* Author:	embest	
* Desc£º	Touch Screen define file
* History:	
*********************************************************************************************/
#ifndef __TP_H__
#define __TP_H__

#include <string.h>
#include "def.h"
#include "44blib.h"
#include "44b.h"
#include "lcd.h"

/*--- global  variables ---*/
volatile int CheckTSP,oneTouch;
unsigned int  Vx, Vy;
unsigned int  Xmax;
unsigned int  Ymax;
unsigned int  Xmin;
unsigned int  Ymin;

/////////////////////////////////////////////////////////////////////
// limites de la pantalla para calibrar
volatile int LIM_ARRIBA_X;
volatile int LIM_ABAJO_X;
volatile int LIM_IZQ_X;
volatile int LIM_DCH_X;
volatile int LIM_ARRIBA_Y;
volatile int LIM_ABAJO_Y;
volatile int LIM_IZQ_Y;
volatile int LIM_DCH_Y;

// para detectar cuando ha habido una pulsacion ///////////////////////////
volatile int contador_pulsaciones;
// guardamos ultima pulsacion
volatile int pulsacion_y;
volatile int pulsacion_x;

int ultima_pulsacion(void);
int pulsacion_X_CORD(void);
int pulsacion_Y_CORD(void);
void TS_Test(void);
void TS_init(void);
void TSInt(void);
void TS_close(void);
void Lcd_TC(void);
void DesignREC(ULONG tx, ULONG ty);
void Check_Sel(void);
void user_irq1(void);

#endif /*__TP_H__*/
