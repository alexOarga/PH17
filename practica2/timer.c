/*********************************************************************************************
 * Fichero:		timer.c
 * Autor:
 * Descrip:		funciones de control del timer0 del s3c44b0x
 * Version:
 *********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer.h"
#include "44b.h"
#include "44blib.h"
#include "button.h"
#include "debugPila.h"

/*--- variables globales ---*/
int switch_leds = 0;

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer_ISR(void) {
	switch_leds = 1;
	rI_ISPC |= BIT_TIMER0;
	////////////////////////////////////////////////////
	// maquina de estados
	switch (estado) {
	case 1:
		if (cuenta_trp > 0) {
			cuenta_trp--;
		} else {
			cuenta_medio=300;
			estado = espera_soltar;
		}
		break;
	case 2:
		if (id_boton == 6 && desplazar_bits(rPDATG, 6) == 1) {
			push_debug(8, 4);
			cuenta_trd = 25;	// inicia cuenta contador
			estado = trd;		// estado 1
		} else if (id_boton == 7 && desplazar_bits(rPDATG, 7) == 1) {
			push_debug(9, 8);
			cuenta_trd = 25;	// inicia cuenta contador
			estado = trd;		// estado 1
		} else {
			if (cuenta_medio <= 0) {
				if (id_boton == 6 && desplazar_bits(rPDATG, 6) == 0) {
					push_debug(1, 4);
					int_count++;
				} else {
					push_debug(2, 8);
					int_count--;
				}
				D8Led_symbol(int_count & 0x000f); // sacamos el valor por pantalla (m�dulo 16)
				cuenta_medio = 300;
			} else {
				cuenta_medio--;
			}
		}
		break;
	case 3:
		if (cuenta_trd > 0) {
			cuenta_trd--;
		} else {
			rINTMSK |= (BIT_TIMER0);
			rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
			rI_ISPC |= BIT_EINT4567;		// borra el bit pendiente en INTPND
			estado = espera;
			rINTMSK &= ~(BIT_EINT4567); // habilitamos pulsador
			rINTMSK &= ~(BIT_TIMER0);
		}
		break;
	default:
		break;
	}
	////////////////////////////////////////////////////

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	// BIT_TIMER0 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer_init(void) {
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_GLOBAL | BIT_TIMER0); // Emascara todas las lineas excepto Timer0 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */pISR_TIMER0 =
			(unsigned) timer_ISR;

	/* Configura el Timer0 */rTCFG0 = 1; // ajusta el preescalado
	rTCFG1 = 0x0; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 65535; // valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 12800; // valor de comparaci�n
	/* establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/rTCON =
			0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON = 0x09;
}
