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
volatile static unsigned int int_count;

/* definimos estados de la maquina */
volatile int espera;
volatile int trp;
volatile int espera_soltar;
volatile int trd;
volatile int aumenta;

/* cuentas del temporizador de los estados trp y trd */
volatile int cuenta_trp;
volatile int cuenta_trd;
volatile int cuenta_medio;

/* identificador del boton pulsado */
volatile int id_boton;

//////////////////////////////////////////////////////////////////////////////

volatile int inicial_juego;
volatile int espera_fila;
volatile int aumenta_fila;
volatile int espera_col;
volatile int aumenta_col;

volatile int estado_juego;

volatile int cuenta_fila;
volatile int cuenta_col;

volatile int eleccion_hecha;
//////////////////////////////////////////////////////////////////////////////

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void Eint4567_init(void);
unsigned int desplazar_bits(unsigned int registro,int pos);


#endif /* _BUTTON_H_ */
