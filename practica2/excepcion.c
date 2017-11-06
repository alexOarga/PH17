/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"
#include "excepcion.h"
#include "8led.h"

/*--- variables globales ---*/


/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void exception_handler(void) __attribute__((interrupt("DABORT")));
void exception_handler(void) __attribute__((interrupt("SWI")));
void exception_handler(void) __attribute__((interrupt("UNDEF")));

extern int CPSR_read();

void error_exc(){
	error_exc();
}

/*--- codigo de las funciones ---*/
void exception_handler(void)
{
	int excep = CPSR_read();
	if(excep == 23){
		D8Led_symbol(10);
	}
	else if(excep == 27){
		D8Led_symbol(14);
	}else{
		D8Led_symbol(15);
	}
	error_exc();
}

void init_exception(void){
	pISR_DABORT = (unsigned) exception_handler;
	pISR_SWI = (unsigned) exception_handler;
	pISR_UNDEF = (unsigned) exception_handler;

	D8Led_init();
}

