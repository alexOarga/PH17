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

/*--- variables globales del m�dulo ---*/
/* int_count la utilizamos para sacar un n�mero por el 8led.
  Cuando se pulsa un bot�n sumamos y con el otro restamos. �A veces hay rebotes! */
static unsigned int int_count = 0;

//////////////////////////////////////////////////////////////////////////////
/* estado actual de la maquina de estados */
int estado = 0;

/* definimos estados de la maquina */
int espera = 0;
int trp = 1;
int espera_soltar = 2;
int trd = 3;

/* cuentas del temporizador de los estados trp y trd */
int cuenta_trp = 100;
int cuenta_trd = 100;
int cuenta_15 = 100;

/* identificador del boton pulsado */
int id_boton = 0;

int pulsado = 0;
int levantado6 = 0;
int levantado7 = 0;
//////////////////////////////////////////////////////////////////////////////

unsigned int desplazar_bits(unsigned int registro,int pos){
	registro = (registro >> pos);
	return registro & 0x1;
}



/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{

	///////////////////////////////////////////////////////
	//rINTMSK &= ~BIT_EINT4567;

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
	D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (m�dulo 16)

	////////////////////////////////////////






	// timer 0 vuelve a habilitar pulsador

	////////////////////////////////////////
		////////////////////////////////////////

		rINTMSK |= BIT_EINT4567;	// dehabilitamos

				// maquina de estados
				switch(estado){
					case 0:
						if(desplazar_bits(rPDATG,6)==0 && desplazar_bits(rINTPND,21)==1){
							id_boton = 6;		// pulsador 1 EXINT 6
							cuenta_trp = 200;	// inicia cuenta contador
							estado = 1;		// estado 1
						}else if(desplazar_bits(rPDATG,7)==0 && desplazar_bits(rINTPND,21)==1){
							id_boton = 7;		// pulsador 2 EXINT 7
							cuenta_trp = 200;	// inicia cuenta contador7
							estado = 1;		// estado 1
						}
						break;
					//case 1:
						// timer.c
					//	break;
					case 2:
						if(desplazar_bits(rPDATG,6)==1 && id_boton==6){
							cuenta_trp = 200;	// inicia cuenta contador
							estado = trd;		// estado 1
						}else if(desplazar_bits(rPDATG,7)==1 && id_boton==7){
							cuenta_trp = 200;	// inicia cuenta contador
							estado = trd;		// estado 1
						}
						break;
					//case 3:
						// timer.c
					//	break;
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
	/* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    = ~(BIT_GLOBAL | BIT_EINT4567 | BIT_TIMER0 |BIT_TIMER2); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

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
