/*********************************************************************************************
* Fichero:		timer.h
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _EST_H_
#define _EST_H_

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

void f_estado();

#endif /* _TIMER_H_ */
