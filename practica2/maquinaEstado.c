/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "maquinaEstado.h"
#include "44b.h"
#include "44blib.h"


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

void f_estado(){
	////////////////////////////////////////
		// maquina de estados
		switch(estado){
			case 0:
				if(pulsado && id_boton==6){
					estado = 1;		// estado 1
					cuenta_trp = 60;	// inicia cuenta contador
				}else if(pulsado && id_boton==7){
					estado = 1;		// estado 1
					cuenta_trp = 60;	// inicia cuenta contador
				}
				break;
			case 1:
				// timer.c
				break;
			case 2:
				if(levantado6 && id_boton==6){
					estado = trd;		// estado 1
					cuenta_trp = 60;	// inicia cuenta contador
				}else if(levantado7 && id_boton==7){
					estado = trd;		// estado 1
					cuenta_trp = 60;	// inicia cuenta contador
				}
				break;
			case 3:
				// timer.c
				break;
			default:
				break;
		}


		////////////////////////////////////////
}
