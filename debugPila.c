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

#define inicio ((_ISR_STARTADDRESS)-0xf00)-(256*3)    		/* STACK APARTADO 7 */
#define fin ((_ISR_STARTADDRESS)-0xf00)-3   		/* c7ff000 */

unsigned int* pos;
unsigned int* actual;

/*--- variables globales ---*/
void debug_init(){
	pos =  (unsigned int*)inicio;
	actual = pos;
}

void push_debug(int ID_evento, int auxData){
	if(pos==(unsigned int*)fin){
		pos = (unsigned int*) inicio;
	}
	pos[0] = ID_evento;
	pos[1] = auxData;
	pos[2] = timer2_leer();
	pos = pos + 3;
}

