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
volatile static unsigned int int_count = 0;

//////////////////////////////////////////////////////////////////////////////
/* estado actual de la maquina de estados */
volatile int estado = 0;

/* definimos estados de la maquina */
volatile int espera = 0;
volatile int trp = 1;
volatile int espera_soltar = 2;
volatile int trd = 3;
volatile int aumenta = 4;

/* cuentas del temporizador de los estados trp y trd */
volatile int cuenta_trp = 25;
volatile int cuenta_trd = 25;
volatile int cuenta_medio = 500;
// 5 - 10ms
// 250		500ms

/* identificador del boton pulsado */
volatile int id_boton = 6;
//////////////////////////////////////////////////////////////////////////////
volatile int inicial_juego = 0;
volatile int espera_fila = 1;
volatile int aumenta_fila = 2;
volatile int espera_col = 3;
volatile int aumenta_col = 4;

volatile int estado_juego = 0;

volatile int cuenta_fila = 0;
volatile int cuenta_col = 0;
//////////////////////////////////////////////////////////////////////////////

unsigned int desplazar_bits(unsigned int registro, int pos) {
	registro = (registro >> pos);
	return registro & 0x1;
}

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Eint4567_ISR(void) {

	///////////////////////////////////////////////////////
	//rINTMSK &= ~BIT_EINT4567;

	/* Identificar la interrupcion (hay dos pulsadores)*/

	int which_int = rEXTINTPND;
	rI_ISPC=0x1;
	rINTMSK |= BIT_EINT4567;
/*
	switch (which_int) {
	case 4:

		break;
	case 8:

		break;
	default:
		break;
	}
	// }
	*/

	////////////////////////////////////////

	// timer 0 vuelve a habilitar pulsador

	switch(estado_juego){
		case 0:
			D8Led_symbol(0x000f);
			estado_juego = espera_fila;
			break;
		case 1:
			if (desplazar_bits(rPDATG, 6) == 0 && desplazar_bits(rINTPND, 21) == 1) {
				cuenta_fila = 0;
				D8Led_symbol(cuenta_fila & 0x000f);
				estado_juego = aumenta_fila;
			}
			break;
		case 2:
			if (desplazar_bits(rPDATG, 6) == 0 && desplazar_bits(rINTPND, 21) == 1) {
				if(cuenta_fila < 7){
					cuenta_fila++;
				}else{
					cuenta_fila = 0;
				}
				D8Led_symbol(cuenta_fila & 0x000f);
			} else if (desplazar_bits(rPDATG, 7) == 0	&& desplazar_bits(rINTPND, 21) == 1) {
				D8Led_symbol(0x000c);
				estado_juego = espera_col;
			}
			break;
		case 3:
			if (desplazar_bits(rPDATG, 6) == 0 && desplazar_bits(rINTPND, 21) == 1) {
				cuenta_col = 0;
				D8Led_symbol(cuenta_col & 0x000f);
				estado_juego = aumenta_col;
			}
			break;
		case 4:
			if (desplazar_bits(rPDATG, 6) == 0 && desplazar_bits(rINTPND, 21) == 1) {
				if(cuenta_col < 7){
					cuenta_col++;
				}else{
					cuenta_col = 0;
				}
				D8Led_symbol(cuenta_col & 0x000f);
			} else if (desplazar_bits(rPDATG, 7) == 0	&& desplazar_bits(rINTPND, 21) == 1) {
				estado_juego = inicial_juego;
			}
			break;
		default:
			break;
	}


	if (desplazar_bits(rPDATG, 6) == 0 && desplazar_bits(rINTPND, 21) == 1) {
		push_debug(6, which_int);
		//int_count++; // incrementamos el contador
		//D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (m�dulo 16)
		id_boton = 6;		// pulsador 1 EXINT 6
		cuenta_trp = 25;	// inicia cuenta contador
		estado = trp;		// estado 1
	} else if (desplazar_bits(rPDATG, 7) == 0	&& desplazar_bits(rINTPND, 21) == 1) {
		//int_count--; // decrementamos el contador
		//D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (m�dulo 16)
		push_debug(7, which_int);
		id_boton = 7;		// pulsador 2 EXINT 7
		cuenta_trp = 25;	// inicia cuenta contador7
		estado = trp;		// estado 1
	}
	else{
		rINTMSK &= ~BIT_EINT4567;
	}

	////////////////////////////////////////

	/* Finalizar ISR */

	rEXTINTPND = 0xf;		// borra los bits en EXTINTPND
	rI_ISPC |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void Eint4567_init(void) {
	/* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
	rI_ISPC = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;    // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = ~(BIT_GLOBAL | BIT_EINT4567 | BIT_TIMER0 | BIT_TIMER2); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

	/* Establece la rutina de servicio para Eint4567 */pISR_EINT4567 =
			(int) Eint4567_ISR;

	/* Configuracion del puerto G */rPCONG = 0xffff; // Establece la funcion de los pines (EINT0-7)
	rPUPG = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222; // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */rI_ISPC |=
			(BIT_EINT4567);
	rEXTINTPND = 0xf;
}
