/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

//////////////////////////////////////////////////////////////////////////////

/* definimos estados de la maquina */

enum estados_boton{
	espera = 0,
	trp = 1,
	espera_soltar = 2,
	trd = 3,
	aumenta = 4
} volatile estado;

/* estado actual de la maquina de estados */

volatile static unsigned int int_count;

/* cuentas del temporizador de los estados trp y trd */

volatile int cuenta_trp;
volatile int cuenta_trd;
volatile int cuenta_medio;



/* identificador del boton pulsado */
volatile int id_boton;

//////////////////////////////////////////////////////////////////////////////
volatile int izq_pulsado;
volatile int dech_pulsado;
//////////////////////////////////////////////////////////////////////////////

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void Eint4567_init(void);
unsigned int desplazar_bits(unsigned int registro,int pos);


#endif /* _BUTTON_H_ */
