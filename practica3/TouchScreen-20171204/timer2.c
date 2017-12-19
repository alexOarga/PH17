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

/*--- variables globales ---*/
int timer2_num = 0;

/* declaración de función que es rutina de servicio de interrupción
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
	timer2_num++;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK &= ~(BIT_GLOBAL | BIT_TIMER2); // Emascara todas las lineas excepto Timer0 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER0 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

	/* Configura el Timer0 */
	rTCFG0 |= 0x0000;   // PREESCALADO ajusta el preescalado
	rTCFG1 |= 0x00;   // DIVISOR selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 65535;// CADA PAR DE REG valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = 12800;// valor de comparación
	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/

}

/**
 * reinicia cuenta de tiempo y comienza a medir
 */
void timer2_empezar(void){

		/* valor inicial de la cuenta*/

		rTCNTB2 = 65535;	// CADA PAR DE REG valor inicial de cuenta (la cuenta es descendente)

		/* iniciar timer (bit 0) con auto-reload (bit 3)*/

		rTCON &= ~(0xF000);
		rTCON |= 0x2000;	// CONTROL TEMP
		rTCON &= ~(0x2000);
		rTCON |= 0x9000;

		timer2_num = 0;

}

int timer2_leer(void){
	int actual = (rTCNTB2-rTCNTO2);
	return ((timer2_num*rTCNTB2)+actual)*0.03;
}
