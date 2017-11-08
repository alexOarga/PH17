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
int estado;

/* definimos estados de la maquina */
int espera;
int trp;
int espera_soltar;
int trd;

/* cuentas del temporizador de los estados trp y trd */
int cuenta_trp;
int cuenta_trd;
int cuenta_15;

/* identificador del boton pulsado */
int id_boton;

int pulsado;
int levantado6;
int levantado7;

//////////////////////////////////////////////////////////////////////////////

/*--- declaracion de funciones visibles del módulo button.c/button.h ---*/
void Eint4567_init(void);

#endif /* _BUTTON_H_ */
