/*********************************************************************************************
* File£º	main.c
* Author:	embest
* Desc£º	c main entry
* History:	
*********************************************************************************************/

/*--- include files ---*/
#include "44blib.h"
#include "44b.h"
#include "tp.h"
#include "lcd.h"

	char yn;
/*--- function declare ---*/
void Main(void);

/*--- extern function ---*/
extern void Lcd_Test();

int LIM_ARRIBA;
int LIM_ABAJO;
int LIM_IZQ;
int LIM_DCH;
int eleccion_hecha = 0;
int cuenta_fila = 0;
int cuenta_col = 0;
int CHAR_HOR = 8;
int CHAR_VER = 16;
int NUM_CASILLAS = 8;
int TAMANYO_CASILLA = 0;

void calibrar(){

	int detectar_pulsacion;

	Lcd_DisplayString(LCD_XSIZE/2, LCD_YSIZE/2, "Pulsar '+'");
	// ultimo contador de pulsaciones
	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(LCD_XSIZE/2, LCD_YSIZE, '+');
	// mientras no varia el numero de pulsaciones
	while(ultima_pulsacion()==detectar_pulsacion){}
	// aqui ya ha pulsado
	LIM_ARRIBA = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(0, LCD_YSIZE/2, '+');
	while(ultima_pulsacion()==detectar_pulsacion){}
	LIM_IZQ = pulsacion_X_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(LCD_XSIZE/2, 0, '+');
	while(ultima_pulsacion()==detectar_pulsacion){}
	LIM_ABAJO = pulsacion_Y_CORD();

	detectar_pulsacion = ultima_pulsacion();
	Lcd_DisplayChar(LCD_XSIZE, LCD_YSIZE/2, '+');
	while(ultima_pulsacion()==detectar_pulsacion){}
	LIM_DCH = pulsacion_X_CORD();
}

void zoom_pulsar(int x, int y){
	for(int i = 0; i < (TABLERO/2)+1, i++){
		//Lcd_Draw_HLine(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor, INT16U usWidth);
	}
	for(int i = 0; i < (TABLERO/2)+1, i++){
		//Lcd_Draw_VLine(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor, INT16U usWidth);
	}
	// DIBUJAR NUMEROS
	detectar_pulsacion = ultima_pulsacion();
	while(ultima_pulsacion()==detectar_pulsacion){}
	int puls_x = pulsacion_X_CORD()*LCD_XSIZE/(LIM_DCH - LIM_IZQ);
	int puls_y = pulsacion_Y_CORD()*LCD_YSIZE/(LIM_ARRIBA - LIM_ABAJO);
	// SI NO PULSA EN LA PARTE DCHA DE LA PANTALLA
	if(puls_x < CHAR_VER + (NUM_CASILLAS*TAMANYO_CASILLA)){
		int nueva_casilla_x = puls_x%(TAMANYO_CASILLA*2);
		int nueva_casilla_y = puls_y%(TAMANYO_CASILLA*2);
		// dibujar ficha
		int intervalo =
		int detenido = 0;
		int inicio_tiempo = timer2_leer();
		detectar_pulsacion = ultima_pulsacion();
		while(timer2_leer()-inicio_tiempo < intervalo && detenido==0){
			// parpadear...
			if(detectar_pulsacion != ultima_pulsacion()){
				detenido = 1;
			}
			if(izq_pulsado == 1){
				izq_pulsado = 0;
				detenido = 1;
			}
			if(dech_pulsado == 1){
				dech_pulsado = 0;
				detenido = 1;
			}
		}
		// si no lo han detenido confirmamos
		if(detenido == 0){
			eleccion_hecha = 1;
			int cuenta_fila = nueva_casilla_x + 1;
			int cuenta_col = nueva_casilla_y + 1;
		}
	}
}

void comprobar_pulsacion(){
	int puls_x = pulsacion_X_CORD()*LCD_XSIZE/(LIM_DCH - LIM_IZQ);
	int puls_y = pulsacion_Y_CORD()*LCD_YSIZE/(LIM_ARRIBA - LIM_ABAJO);

	if(puls_y < CHAR_VER){
		eleccion_hecha = 1;
		int cuenta_fila = 8;
		int cuenta_col = 8;
	}else{
		// SI NO PULSA EN LA PARTE DCHA DE LA PANTALLA
		if(puls_x < CHAR_VER + (NUM_CASILLAS*TAMANYO_CASILLA)){
			if(puls_y < LCD_YSIZE){
				if(puls_x < (CHAR_VER + (NUM_CASILLAS*TAMANYO_CASILLA))/2){
					zoom_pulsar(0, 4);
				}else{
					zoom_pulsar(4, 4);
				}
			}else{
				if(puls_x < (CHAR_VER + (NUM_CASILLAS*TAMANYO_CASILLA))/2){
					zoom_pulsar(0, 0);
				}else{
					zoom_pulsar(4, 0);
				}
			}
		}
	}

}

void esperar_mov()
{
 //   while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)
 //   *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
	int pantalla_pulsada = 5;


	int detectar_pulsacion = ultima_pulsacion();
	while(eleccion_hecha==0){
	  if(detectar_pulsacion != ultima_pulsacion()){
		  estado_juego = pantalla_pulsada;
	  }
	  switch(estado_juego){
	    case 0:
	      if(izq_pulsado == 1 || dech_pulsado == 1){
  			  izq_pulsado = 0;
  			  dech_pulsado = 0;
	    	  eleccion_hecha = 0;
  			  D8Led_symbol(0x000f);
  			  estado_juego = espera_fila;
	      }
			  break;
	    case 1:
	      if(izq_pulsado == 1){
	    	  	  izq_pulsado = 0;
	    	  	  cuenta_fila = 0;
				  D8Led_symbol(cuenta_fila & 0x000f);
				  estado_juego = aumenta_fila;
	      }
	      break;
	    case 2:
	      if(izq_pulsado == 1){
	    	  izq_pulsado = 0;
	    	  if(cuenta_fila < 7){
  					cuenta_fila++;
  				}else{
  					cuenta_fila = 0;
  				}
  				D8Led_symbol(cuenta_fila & 0x000f);
	      }else if(dech_pulsado == 1){
	    	  	dech_pulsado = 0;
  				D8Led_symbol(0x000c);
  				estado_juego = espera_col;
			  }
	      break;
	    case 3:
  			if (izq_pulsado == 1) {
  				izq_pulsado = 0;
  				cuenta_col = 0;
  				D8Led_symbol(cuenta_col & 0x000f);
  				estado_juego = aumenta_col;
  			}
  			break;
  		case 4:
  			if (izq_pulsado == 1) {
  				izq_pulsado = 0;
  				if(cuenta_col < 7){
  					cuenta_col++;
  				}else{
  					cuenta_col = 0;
  				}
  				D8Led_symbol(cuenta_col & 0x000f);
  			} else if (dech_pulsado == 1) {
  				dech_pulsado = 0;
  				eleccion_hecha = 1;
  				estado_juego = inicial_juego;
  			}
  			break;
  		case 5:	// pantalla pulsada
  			int estado anterior = estado_juego;
  			comprobar_pulsacion();
  			estado_juego = estado_anterior;
  			break;
  		default:
  			break;
	  }
	}

	  eleccion_hecha = 0;


}

/*--- function code ---*/
/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Main(void)
{
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
    _Link();           /* Print Misc info */
    /******************/
	/* user interface */
	/******************/
	Lcd_Test();
	TS_Test();
		
	while(1)
	 {
	   yn = Uart_Getch();
	   
	   if(yn == 0x52) TS_Test();// R to reset the XY REC
	   //else break;
	 }
	
	TS_close();
}
