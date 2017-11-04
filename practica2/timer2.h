/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*--- variables visibles del módulo timer.c/timer.h ---*/
int timer2_num;

/*--- declaracion de funciones visibles del módulo timer.c/timer.h ---*/
void timer2_inicializar(void);

void timer2_empezar(void);

int timer2_leer(void);

#endif /* _TIMER_H_ */
