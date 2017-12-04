/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "timer.h"
#include "timer2.h"
#include "excepcion.h"
#include "led.h"
#include "debugPila.h"
#include "8led.h"
#include "44b.h"
#include "44blib.h"

#define PLACA 1

/*--- variables globales ---*/


/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	sys_init();         // Inicializacion de la placa, interrupciones y puertos
	timer_init();	    // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se ver� reflejado en el 8led
	D8Led_init();       // inicializamos el 8led
	debug_init();
	init_exception();

	timer2_inicializar();

	/* Valor inicial de los leds */
	leds_off();
	led1_on();

/*
	while (1)
	{

		if (switch_leds == 1)
		{
			timer2_empezar();
			Delay(100);
			int y = timer2_leer();
			timer2_empezar();
			Delay(1);
			y = timer2_leer();
			if(y>0)y-=1;
			switch_leds = 0;
		}
		int estadolec = estado;
	}
*/

	reversi8();
}
