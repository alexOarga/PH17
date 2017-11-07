/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "debugPila.h"

/*--- variables globales del módulo ---*/
/* int_count la utilizamos para sacar un número por el 8led.
  Cuando se pulsa un botón sumamos y con el otro restamos. ¡A veces hay rebotes! */
static unsigned int int_count = 0;

//////////////////////////////////////////////////////////////////////////////
/* estado actual de la maquina de estados */
volatile int estado = 0;

/* definimos estados de la maquina */
int espera = 0;
int trp = 1;
int espera_soltar = 2;
int trd = 3;

/* cuentas del temporizador de los estados trp y trd */
volatile int cuenta_trp;
volatile int cuenta_trd;
volatile int cuenta_15;

/* identificador del boton pulsado */
int id_boton;

//////////////////////////////////////////////////////////////////////////////

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{

	/* deshabilitamos interrupciones pulsadores */
	rINTMSK |= BIT_EINT4567;

	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;
	push_debug(8,which_int);
	switch (which_int)
	{
		case 4:
			int_count++; // incrementamos el contador
			break;
		case 8:
			int_count--; // decrementamos el contador
			break;
		default:
			break;
	}
	// }
	D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (módulo 16)

	////////////////////////////////////////
	// maquina de estados
	switch(estado){
		case 0:
			if(rEXTINTPND[2]==1 && rINTPND[21]==1){
				id_boton = 6;		// pulsador 1 EXINT 6
				estado = 1;		// estado 1
				cuenta_trp = 200;	// inicia cuenta contador
			}else if(rEXTINTPND[3]==1 && rINTPND[21]==1){
				id_boton = 7;		// pulsador 2 EXINT 7
				estado = 1;		// estado 1
				cuenta_trp = 200;	// inicia cuenta contador
			}
			break;
		case 1:
			// contador 0 pasa al siguiente estado cuando cuenta_trp es 0
			break;
		case 2:
			// TODO
			break;
		case 3:
			// contador 0 pasa al siguiente estado (0) cuando cuenta_trd es 0
			break;
		default:
			break;
	}


	////////////////////////////////////////

	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    = ~(BIT_GLOBAL | BIT_EINT4567 | BIT_TIMER0); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;
}
