/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

//////////////////////////////////////////////////////////////////////////////
/* estado actual de la maquina de estados */
volatile int estado;

/* definimos estados de la maquina */
volatile int espera;
volatile int trp;
volatile int espera_soltar;
volatile int trd;

/* cuentas del temporizador de los estados trp y trd */
volatile int cuenta_trp;
volatile int cuenta_trd;

/* identificador del boton pulsado */
volatile int id_boton;

//////////////////////////////////////////////////////////////////////////////

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void Eint4567_init(void);
unsigned int desplazar_bits(unsigned int registro,int pos);


#endif /* _BUTTON_H_ */
