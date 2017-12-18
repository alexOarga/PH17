/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _EXC_H_
#define _EXC_H_

void exception_handler(void);
void init_exception(void);
extern int CPSR_read();



#endif /* _TIMER_H_ */
