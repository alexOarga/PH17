/*********************************************************************************************
* File£º	tp.H
* Author:	embest	
* Desc£º	Touch Screen define file
* History:	
*********************************************************************************************/
#include "def.h"
#include "44b.h"
#include "44blib.h"
#ifndef __TP_H__
#define __TP_H__

#endif /*__TP_H__*/

/*--- global  variables ---*/
volatile int CheckTSP,oneTouch;
unsigned int  Vx, Vy;
unsigned int  Xmax;
unsigned int  Ymax;
unsigned int  Xmin;
unsigned int  Ymin;

// para detectar cuando ha habido una pulsacion ///////////////////////////
int contador_pulsaciones = 0;
// guardamos ultima pulsacion
int pulsacion_y = 0;
int pulsacion_x = 0;

void TS_Test(void);
void TS_init(void);
void TSInt(void);
void TS_close(void);
void Lcd_TC(void);
void DesignREC(ULONG tx, ULONG ty);
void Check_Sel(void);
void user_irq1(void);
