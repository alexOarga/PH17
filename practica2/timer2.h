/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

/*--- variables visibles del módulo timer.c/timer.h ---*/
int timer2_num;

/*--- declaracion de funciones visibles del módulo timer.c/timer.h ---*/
void timer2_inicializar();

void timer2_empezar();

int timer2_leer();

#endif /* _TIMER_H_ */
