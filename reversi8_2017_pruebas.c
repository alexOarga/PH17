
// Tamaño del tablero
#define DIM   8

// Valores que puede devolver la función patron_volteo())
#define PATRON_ENCONTRADO    1
#define NO_HAY_PATRON        0

// Estados de las casillas del tablero
#define CASILLA_VACIA 0
#define FICHA_BLANCA  1
#define FICHA_NEGRA   2

// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
#define NO               0
#define SI               1
#define CASILLA_OCUPADA  2

#define CHAR_VER 16
#define CHAR_HOR 8

enum maquina_est_juego {
	inicial_juego = 0,
	espera_fila = 1,
	aumenta_fila = 2,
	espera_col = 3,
	aumenta_col = 4,
	zoom = 5,
	zoom_pulsado = 6,
	eleccion_ya_hecha = 7
};

volatile int estado_juego = 0;

volatile int cuenta_fila = 0;
volatile int cuenta_col = 0;

//////////////////////////////////////////////////////
// eleccion de casilla con botnes o pantalla
volatile int eleccion_hecha = 0;
volatile int tamano_casilla = 0;
volatile int tiempo_total = 0;
volatile int tiempo_calculos = 0;

volatile int x = 0;
volatile int y = 0;

#include "button.h"
#include "led.h"
#include "lcd.h"
#include "tp.h"

/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean más fáciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const char __attribute__ ((aligned (8))) tabla_valor[8][8] = { { 8, 2, 7,
		3, 3, 7, 2, 8 }, { 2, 1, 4, 4, 4, 4, 1, 2 }, { 7, 4, 6, 5, 5, 6, 4, 7 },
		{ 3, 4, 5, 0, 0, 5, 4, 3 }, { 3, 4, 5, 0, 0, 5, 4, 3 }, { 7, 4, 6, 5, 5,
				6, 4, 7 }, { 2, 1, 4, 4, 4, 4, 1, 2 },
		{ 8, 2, 7, 3, 3, 7, 2, 8 } };

// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
const char vSF[DIM] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const char vSC[DIM] = { 0, 1, 1, 1, 0, -1, -1, -1 };

//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deberían serlo
// tablero, fila, columna y ready son varibles que se deberían definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea más fácil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
char __attribute__ ((aligned (8))) tablero[DIM][DIM] = { { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA }, { CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA,
		CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA } };

////////////////////////////////////////////////////////////////////
// VARIABLES PARA INTERACCIONAR CON LA ENTRADA SALIDA
// Pregunta: ¿hay que hacer algo con ellas para que esto funcione bien?
// (por ejemplo añadir alguna palabra clave para garantizar que la sincronización a través de esa variable funcione)
char fila = 0, columna = 0, ready = 0;

// extern int patron_volteo(char tablero[][8], int *longitud,char f, char c, char SF, char SC, char color);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM], char candidatas[][DIM]) {
	int i, j;

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++)
			tablero[i][j] = CASILLA_VACIA;
	}
	tablero[3][3] = FICHA_BLANCA;
	tablero[4][4] = FICHA_BLANCA;
	tablero[3][4] = FICHA_NEGRA;
	tablero[4][3] = FICHA_NEGRA;

	candidatas[3][3] = CASILLA_OCUPADA;
	candidatas[4][4] = CASILLA_OCUPADA;
	candidatas[3][4] = CASILLA_OCUPADA;
	candidatas[4][3] = CASILLA_OCUPADA;

	// casillas a explorar:
	candidatas[2][2] = SI;
	candidatas[2][3] = SI;
	candidatas[2][4] = SI;
	candidatas[2][5] = SI;
	candidatas[3][2] = SI;
	candidatas[3][5] = SI;
	candidatas[4][2] = SI;
	candidatas[4][5] = SI;
	candidatas[5][2] = SI;
	candidatas[5][3] = SI;
	candidatas[5][4] = SI;
	candidatas[5][5] = SI;
}

void display_zoom(void) {
	Lcd_Clr();
	Lcd_Active_Clr();
	display_cuadricula(CHAR_HOR, CHAR_VER, DIM / 2, tamano_casilla * 2, x, y);
	dibujar_fichas_tablero(tablero, x, y, DIM / 2, CHAR_HOR, CHAR_VER,
			tamano_casilla * 2);
	Lcd_Dma_Trans();
}

void zoom_pulsar(int puls_x, int puls_y) {
	// calcular posicion en tablero completo (8, 8)
	int nueva_casilla_x = (puls_x - CHAR_HOR) / (tamano_casilla * 2);
	puls_y = LCD_YSIZE - puls_y;
	int nueva_casilla_y = (puls_y - CHAR_VER) / (tamano_casilla * 2);
	nueva_casilla_x += x;
	nueva_casilla_y += y;

	// dibuja tablero completo
	display_tablero();
	// dibuja ficha
	dibujar_ficha(BLACK, (nueva_casilla_x * tamano_casilla) + CHAR_HOR,
			(nueva_casilla_y * tamano_casilla) + CHAR_VER, tamano_casilla);
	Lcd_Dma_Trans();

	// varaibles detencion
	int intervalo = 2000000;
	int inicio_tiempo =timer2_leer();

	// varaibles parpadeo
	int intervalo_parapdeo = 500000;
	int inicio_parpadeo = timer2_leer();
	int esta_oculto = 0;

	int detenido = 0;
	int detectar_pulsacion = ultima_pulsacion();
	int actual_time=0;

	do {
		// parpadear...

		if (actual_time - inicio_parpadeo > intervalo_parapdeo) {
			inicio_parpadeo = timer2_leer(); // REINICIAR CONTADOR
			if (esta_oculto == 0) {	// ocultar
				dibujar_ficha(WHITE,
						(nueva_casilla_x * tamano_casilla) + CHAR_HOR,
						(nueva_casilla_y * tamano_casilla) + CHAR_VER,
						tamano_casilla);
				Lcd_Dma_Trans();
				esta_oculto = 1;
			} else {					// mostrar
				dibujar_ficha(BLACK,
						(nueva_casilla_x * tamano_casilla) + CHAR_HOR,
						(nueva_casilla_y * tamano_casilla) + CHAR_VER,
						tamano_casilla);
				Lcd_Dma_Trans();
				esta_oculto = 0;
			}
		}

		if (detectar_pulsacion != ultima_pulsacion()) {
			detenido = 1;
		}
		if (izq_pulsado == 1) {
			izq_pulsado = 0;
			detenido = 1;
		}
		if (dech_pulsado == 1) {
			dech_pulsado = 0;
			detenido = 1;
		}
		actual_time=timer2_leer();
	} while ((actual_time - inicio_tiempo < intervalo) && detenido == 0);

	// si no lo han detenido confirmamos
	if (detenido == 0) {
		eleccion_hecha = 1;
		cuenta_fila = nueva_casilla_x;
		cuenta_col = nueva_casilla_y;
	}

}

int pulsa_en_pasar(int puls_x, int puls_y) {
	if (puls_y < CHAR_VER) {
		return 1;
	} else {
		return 0;
	}
}

int pulsa_en_tablero(int puls_x, int puls_y) {
	if (puls_x < CHAR_HOR + (DIM * tamano_casilla)) {
		return 1;
	} else {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// comprueba si se ha pulsado para hacer zoom o fuera del tablero
void comprobar_pulsacion_zoom(int puls_x, int puls_y) {

	// SI PULSA bAJO
	if (puls_y < CHAR_VER + ((DIM * tamano_casilla)) / 2) {
		if (puls_x < CHAR_HOR + ((DIM * tamano_casilla)) / 2) {
			x = 0;
			y = 4;
			display_zoom();
		} else {
			x = 4;
			y = 4;
			display_zoom();
		}
		// SI PULSA ARRIBA
	} else {
		if (puls_x < CHAR_HOR + ((DIM * tamano_casilla)) / 2) {
			x = 0;
			y = 0;
			display_zoom();
		} else {
			x = 4;
			y = 0;
			display_zoom();
		}
	}
}

void dibujar_ficha(INT8U ucColor, int coor_x, int coor_y, int tamanyo) {
	LcdClrRect(coor_x + 2, coor_y + 2, (coor_x) + (tamanyo - 2),
			(coor_y) + (tamanyo - 2), ucColor);
}

// inicio_coor_x, inicio_coor_y : posicion de la esquina superior izquirda del tablero a mostrar
// dimension es altura y anchura del tablero en casillas (8 o 4 con zoom)
// inicio_x, inicio_y: poscion del tablero desde donde se muestra (para zoom)
void dibujar_fichas_tablero(char tablero[][DIM], int inicio_x, int inicio_y,
		int dimension, int inicio_coor_x, int inicio_coor_y,
		int tamanyo_casilla) {
	int i = 0;
	int j = 0;
	for (i = 0; i < dimension; i++) {
		for (j = 0; j < dimension; j++) {
			if (tablero[inicio_x + i][inicio_y + j] == FICHA_BLANCA) {
				dibujar_ficha(LIGHTGRAY, inicio_coor_x + (tamanyo_casilla * i),
						inicio_coor_y + (tamanyo_casilla * j), tamanyo_casilla);
			}
			if (tablero[inicio_x + i][inicio_y + j] == FICHA_NEGRA) {
				dibujar_ficha(BLACK, inicio_coor_x + (tamanyo_casilla * i),
						inicio_coor_y + (tamanyo_casilla * j), tamanyo_casilla);
			}
		}
	}
}

void display_tiempo(int coor_x, int coor_y, int lon) {
	INT8U* tiempo = "Tiempo";
	INT8U* total = "total:";
	INT8U* calculos = "calculos:";
	INT8U total_ch[] = {'0' +  tiempo_total};
	INT8U calc_ch[] = {'0' + tiempo_calculos};

	// BORRAMOS ESA ZONA DE LA PANTALLA
	LcdClrRect(coor_x, coor_y, coor_x + (lon * CHAR_HOR),
			coor_y + (CHAR_VER * 7), WHITE);

	Lcd_DspAscII8x16(coor_x, coor_y, BLACK, tiempo);
	Lcd_DspAscII8x16(coor_x, coor_y + CHAR_VER, BLACK, total);
	Lcd_DspAscII8x16(coor_x, coor_y + (CHAR_VER) * 2, BLACK, total_ch);

	Lcd_DspAscII8x16(coor_x, coor_y + (CHAR_VER) * 4, BLACK, tiempo);
	Lcd_DspAscII8x16(coor_x, coor_y + (CHAR_VER) * 5, BLACK, calculos);
	Lcd_DspAscII8x16(coor_x, coor_y + (CHAR_VER) * 6, BLACK, calc_ch);
}

void display_cuadricula(int coor_x, int coor_y, int dimension, int tamanyo,
		int inicio_hor, int inicio_ver) {
	char inicio_numero_hor = '0' + inicio_hor;
	char inicio_numero_ver = '0' + inicio_ver;
	int i = 0;
	for (i = 0; i < dimension + 1; i++) {	// verticales
		if (i < dimension) {
			char disp[] = { i + inicio_numero_ver, 0x0 };
			Lcd_DspAscII8x16(0, (tamanyo * i) + CHAR_VER, BLACK, disp);
		}
		//Lcd_Draw_Line(CHAR_HOR, (tamanyo*i)+CHAR_VER, , , 10, 2);
		Lcd_Draw_VLine(coor_y, (tamanyo * dimension) + coor_y,
				(tamanyo * i) + coor_x, 10, 2);

	}
	for (i = 0; i < dimension + 1; i++) {	// horizontales
		if (i < dimension) {
			char disp[] = { i + inicio_numero_hor, 0x0 };
			Lcd_DspAscII8x16((tamanyo * i) + CHAR_HOR, 0, BLACK, disp);
		}
		//Lcd_Draw_Line((tamanyo*i)+CHAR_HOR,CHAR_VER, (tamanyo*i)+CHAR_HOR, (tamanyo*dimension)+CHAR_VER,  10, 2);
		Lcd_Draw_HLine(coor_x, (tamanyo * dimension) + coor_x,
				(tamanyo * i) + coor_y, 10, 2);
	}
}

/*********************************************************************************************
 *********************************************************************************************/
void display_tablero(void) {

	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	volatile INT8U* pucChar1 = "Pulse o introduzca (8,8)";
	Lcd_DspAscII8x16(0, LCD_YSIZE - CHAR_VER, BLACK, pucChar1);
	tamano_casilla = (LCD_YSIZE - 2*CHAR_VER) / DIM;
	if (LCD_YSIZE < LCD_XSIZE) {
		display_cuadricula(CHAR_HOR, CHAR_VER, DIM, tamano_casilla, 0, 0);
	}
	dibujar_fichas_tablero(tablero, 0, 0, DIM, CHAR_HOR, CHAR_VER,
			tamano_casilla);
	display_tiempo((CHAR_HOR * 4) + (tamano_casilla * DIM), CHAR_VER, 9);
	Lcd_Dma_Trans();
}

void pantalla_inicial() {
	Lcd_Clr();
	Lcd_Active_Clr();
	volatile INT8U* pucChar1 = "Toque la pantalla para jugar";
	Lcd_DspAscII8x16((LCD_XSIZE / 2) - (CHAR_HOR * 13), LCD_YSIZE / 2, BLACK,
			pucChar1);
	Lcd_Dma_Trans();
	int detectar_pulsacion = ultima_pulsacion();
	while (detectar_pulsacion == ultima_pulsacion() && izq_pulsado == 0
			&& dech_pulsado == 0) {
		// esperamos a que pulse
	}
	if (izq_pulsado == 1) {
		izq_pulsado = 0;
	}
	if (dech_pulsado == 1) {
		dech_pulsado = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Espera a que ready valga 1.
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionará.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria 

void esperar_mov() {
	//   while (*ready == 0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)
	//   *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
	eleccion_hecha = 0;
	int esta_en_zoom = 0;
	int detectar_pulsacion = ultima_pulsacion();
	int puls_x = 0;
	int puls_y = 0;
	int estado_anterior;

	// actualiza el tablero
	display_tablero();
	tiempo_total=timer2_leer();
	tiempo_total/=1000000;
	int anterior_tiempo=tiempo_total;
	while (eleccion_hecha == 0) {
		////////////////////////////////////////////////////////////////////
		if(tiempo_total!=anterior_tiempo){
			display_tiempo((CHAR_HOR * 4) + (tamano_casilla * DIM), CHAR_VER, 9);
			Lcd_Dma_Trans();
		}

		if (detectar_pulsacion != ultima_pulsacion()) {	// se ha pulsado la pantalla
			puls_x = (pulsacion_X_CORD() - Xmin) * LCD_XSIZE / (Xmax - Xmin);
			puls_y = (pulsacion_Y_CORD() - Ymin) * LCD_YSIZE / (Ymax - Ymin);
			if (pulsa_en_pasar(puls_x, puls_y) == 1) {
				eleccion_hecha = 1;
				cuenta_fila = 8;
				cuenta_col = 8;
				estado_juego = eleccion_ya_hecha;
			}
			if (pulsa_en_tablero(puls_x, puls_y) == 1 && !esta_en_zoom) {
				// guardamos estado al que volver
				estado_anterior = estado_juego;
				esta_en_zoom = 1;
				estado_juego = zoom;
			} else {
				if (pulsa_en_tablero(puls_x, puls_y) == 1 && esta_en_zoom) {
					esta_en_zoom = 0;
					estado_juego = zoom_pulsado;
				}else if(pulsa_en_tablero(puls_x, puls_y) == 0 && esta_en_zoom){
					display_tablero();
					esta_en_zoom = 0;
				}
			}

			// actualizamos para que no vuelva a entrar
			detectar_pulsacion = ultima_pulsacion();
		}
		///////////////////////////////////////////////////////////////////

		switch (estado_juego) {
		case 0:
			if (izq_pulsado == 1 || dech_pulsado == 1) {
				izq_pulsado = 0;
				dech_pulsado = 0;
				eleccion_hecha = 0;
				D8Led_symbol(0x000f);
				estado_juego = espera_fila;
			}
			break;
		case 1:
			if (izq_pulsado == 1) {
				izq_pulsado = 0;
				cuenta_fila = 0;
				D8Led_symbol(cuenta_fila & 0x000f);
				estado_juego = aumenta_fila;
			}
			break;
		case 2:
			if (izq_pulsado == 1) {
				izq_pulsado = 0;
				if (cuenta_fila < 8) {
					cuenta_fila++;
				} else {
					cuenta_fila = 0;
				}
				D8Led_symbol(cuenta_fila & 0x000f);
			} else if (dech_pulsado == 1) {
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
				if (cuenta_col < 8) {
					cuenta_col++;
				} else {
					cuenta_col = 0;
				}
				D8Led_symbol(cuenta_col & 0x000f);
			} else if (dech_pulsado == 1) {
				dech_pulsado = 0;
				if (cuenta_col != cuenta_fila
						&& (cuenta_col == 8 || cuenta_fila == 8)) {
					// datos incorrectos
					cuenta_col = 0;
					cuenta_fila = 0;
				} else {
					eleccion_hecha = 1;
				}
				estado_juego = inicial_juego;
			}
			break;
		case 5:	/////////////////////////////// estado zoom
			comprobar_pulsacion_zoom(puls_x, puls_y); // comprueba el recuadro pulsado (x,y)
			display_zoom();	 // dibuja el reacuadro (0,0)(0,4)(4,0)(4,4)
			estado_juego = estado_anterior;
			break;
		case 6: /////////////////////////////// estado zoom pulsado
			zoom_pulsar(puls_x, puls_y);
			estado_juego = inicial_juego;
			break;
		case 7: /////////////////////////////// estado eleccion ya hecha
			// no se hace nada
			break;
		default:
			break;
		}
		anterior_tiempo=tiempo_total;
		tiempo_total=timer2_leer();
		tiempo_total/=1000000;
	}

	eleccion_hecha = 0;

}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida) {
	char ficha;

	// ficha = tablero[f][c];
	// no puede accederse a tablero[f][c]
	// ya que algún índice puede ser negativo

	if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0)
			&& (tablero[f][c] != CASILLA_VACIA)) {
		*posicion_valida = 1;
		ficha = tablero[f][c];
	} else {
		*posicion_valida = 0;
		ficha = CASILLA_VACIA;
	}
	return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
extern char ficha_valida_arm(char tablero[][DIM], char f, char c,
		int *posicion_valida);

////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
extern char ficha_valida_thumb(char tablero[][DIM], char f, char c,
		int *posicion_valida);

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
extern int patron_volteo_arm_c(char tablero[][DIM], int *longitud, char FA,
		char CA, char SF, char SC, char color);

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
extern int patron_volteo_arm_thumb(char tablero[][DIM], int *longitud, char FA,
		char CA, char SF, char SC, char color);

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
extern int patron_volteo_arm_arm(char tablero[][DIM], int *longitud, char FA,
		char CA, char SF, char SC, char color);

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo 
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
int patron_volteo_c_c(char tablero[][DIM], int *longitud, char FA, char CA,
		char SF, char SC, char color) {
	int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
	int patron; //indica si se ha encontrado un patrón o no
	char casilla;   // casilla es la casilla que se lee del tablero
	FA = FA + SF;
	CA = CA + SC;

	casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	// mientras la casilla está en el tablero, no está vacía,
	// y es del color rival seguimos buscando el patron de volteo
	if ((posicion_valida == 1) && (casilla != color)) {
		*longitud = *longitud + 1;
		patron = patron_volteo_c_c(tablero, longitud, FA, CA, SF, SC, color);
		return patron;
	}
	// si la ultima posición era válida y la ficha es del jugador actual,
	// entonces hemos encontrado el patrón
	else if ((posicion_valida == 1) && (casilla == color)) {
		if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
				{
			return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
			//printf("PATRON_ENCONTRADO \n");
		} else {
			return NO_HAY_PATRON; // si no hay que voltear no hay patrón
			//printf("NO_HAY_PATRON \n");
		}
	}
	// en caso contrario es que no hay patrón
	else {
		return NO_HAY_PATRON;
		//printf("NO_HAY_PATRON \n");
	}
}
////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
int patron_volteo_c_arm(char tablero[][DIM], int *longitud, char FA, char CA,
		char SF, char SC, char color) {
	int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
	int patron; //indica si se ha encontrado un patrón o no
	char casilla;   // casilla es la casilla que se lee del tablero
	FA = FA + SF;
	CA = CA + SC;

	casilla = ficha_valida_arm(tablero, FA, CA, &posicion_valida);
	// mientras la casilla está en el tablero, no está vacía,
	// y es del color rival seguimos buscando el patron de volteo
	if ((posicion_valida == 1) && (casilla != color)) {
		*longitud = *longitud + 1;
		patron = patron_volteo_c_arm(tablero, longitud, FA, CA, SF, SC, color);
		return patron;
	}
	// si la ultima posición era válida y la ficha es del jugador actual,
	// entonces hemos encontrado el patrón
	else if ((posicion_valida == 1) && (casilla == color)) {
		if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
				{
			return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
			//printf("PATRON_ENCONTRADO \n");
		} else {
			return NO_HAY_PATRON; // si no hay que voltear no hay patrón
			//printf("NO_HAY_PATRON \n");
		}
	}
	// en caso contrario es que no hay patrón
	else {
		return NO_HAY_PATRON;
		//printf("NO_HAY_PATRON \n");
	}
}
////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo es una función recursiva que busca el patrón de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando. Se usa para saber cuantas fichas habría que voltear
int patron_volteo_c_thumb(char tablero[][DIM], int *longitud, char FA, char CA,
		char SF, char SC, char color) {
	int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
	int patron; //indica si se ha encontrado un patrón o no
	char casilla;   // casilla es la casilla que se lee del tablero
	FA = FA + SF;
	CA = CA + SC;

	casilla = ficha_valida_thumb(tablero, FA, CA, &posicion_valida);
	// mientras la casilla está en el tablero, no está vacía,
	// y es del color rival seguimos buscando el patron de volteo
	if ((posicion_valida == 1) && (casilla != color)) {
		*longitud = *longitud + 1;
		patron = patron_volteo_c_thumb(tablero, longitud, FA, CA, SF, SC,
				color);
		return patron;
	}
	// si la ultima posición era válida y la ficha es del jugador actual,
	// entonces hemos encontrado el patrón
	else if ((posicion_valida == 1) && (casilla == color)) {
		if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
				{
			return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
			//printf("PATRON_ENCONTRADO \n");
		} else {
			return NO_HAY_PATRON; // si no hay que voltear no hay patrón
			//printf("NO_HAY_PATRON \n");
		}
	}
	// en caso contrario es que no hay patrón
	else {
		return NO_HAY_PATRON;
		//printf("NO_HAY_PATRON \n");
	}
}
////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM], char FA, char CA, char SF, char SC, int n,
		char color) {
	int i;

	for (i = 0; i < n; i++) {
		FA = FA + SF;
		CA = CA + SC;
		tablero[FA][CA] = color;
	}
}
////////////////////////////////////////////////////////////////////////////////
// funcion de bloqueo para detectar error
void error_actualizar() {
	error_actualizar();
}

////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};    
int actualizar_tablero(char tablero[][DIM], char f, char c, char color) {
	char SF, SC; // cantidades a sumar para movernos en la dirección que toque
	int i;
	int flip_c_c, flip_c_arm, flip_c_thumb, flip_arm_c, flip_arm_arm,
			flip_arm_thumb;
	int patron_c_c, patron_c_arm, patron_c_thumb, patron_arm_c, patron_arm_arm,
			patron_arm_thumb;

	for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
			{
		SF = vSF[i];
		SC = vSC[i];
		// flip: numero de fichas a voltear
		flip_c_c = flip_c_arm = flip_c_thumb = flip_arm_c = flip_arm_arm =
				flip_arm_thumb = 0;

		patron_c_c = patron_volteo_c_c(tablero, &flip_c_c, f, c, SF, SC, color);
		//patron_c_arm = patron_volteo_c_arm(tablero,&flip_c_arm, f, c, SF, SC, color);
		//patron_c_thumb = patron_volteo_c_thumb(tablero,&flip_c_thumb, f, c, SF, SC, color);
		//patron_arm_c = patron_volteo_arm_c(tablero,&flip_arm_c, f, c, SF, SC, color);
		//patron_arm_arm = patron_volteo_arm_arm(tablero,&flip_arm_arm, f, c, SF, SC, color);
		//patron_arm_thumb = patron_volteo_arm_thumb(tablero,&flip_arm_thumb, f, c, SF, SC, color);
		/*
		 if  (
		 (patron_c_c == patron_c_arm == patron_c_thumb == patron_arm_c == patron_arm_arm == patron_arm_thumb == PATRON_ENCONTRADO)
		 )
		 {
		 //bien
		 }else{
		 patron_c_c = patron_volteo_c_c(tablero,&flip_c_c, f, c, SF, SC, color);
		 patron_c_arm = patron_volteo_c_arm(tablero,&flip_c_arm, f, c, SF, SC, color);
		 patron_c_thumb = patron_volteo_c_thumb(tablero,&flip_c_thumb, f, c, SF, SC, color);
		 patron_arm_c = patron_volteo_arm_c(tablero,&flip_arm_c, f, c, SF, SC, color);
		 patron_arm_arm = patron_volteo_arm_arm(tablero,&flip_arm_arm, f, c, SF, SC, color);
		 patron_arm_thumb = patron_volteo_arm_thumb(tablero,&flip_arm_thumb, f, c, SF, SC, color);

		 error_actualizar();
		 }
		 */
		//printf("Flip: %d \n", flip);
		if (patron_c_c == PATRON_ENCONTRADO) {
			voltear(tablero, f, c, SF, SC, flip_c_c, color);
		}
		if (patron_c_arm == PATRON_ENCONTRADO) {
			voltear(tablero, f, c, SF, SC, flip_c_arm, color);
		}
		if (patron_c_thumb == PATRON_ENCONTRADO) {
			voltear(tablero, f, c, SF, SC, flip_c_thumb, color);
		}
		if (patron_arm_c == PATRON_ENCONTRADO) {
			voltear(tablero, f, c, SF, SC, flip_arm_c, color);
		}
		if (patron_arm_arm == PATRON_ENCONTRADO) {
			voltear(tablero, f, c, SF, SC, flip_arm_arm, color);
		}
		if (patron_arm_thumb == PATRON_ENCONTRADO) {
			voltear(tablero, f, c, SF, SC, flip_arm_thumb, color);
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// funcion de bloqueo para detectar error
void error_elegirmov() {
	error_elegirmov();
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posición si se puede mover
// En caso afirmativo, consulta la puntuación de la posición y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(char candidatas[][DIM], char tablero[][DIM], char *f, char *c,
		char tipoficha) {
	int i, j, k, found;
	int mf = -1; // almacena la fila del mejor movimiento encontrado
	int mc;      // almacena la columna del mejor movimiento encontrado
	char mejor = 0; // almacena el mejor valor encontrado
	int longitud_c_c, longitud_c_arm, longitud_c_thumb, longitud_arm_c,
			longitud_arm_arm, longitud_arm_thumb;
	int patron_c_c, patron_c_arm, patron_c_thumb, patron_arm_c, patron_arm_arm,
			patron_arm_thumb;
	char SF, SC; // cantidades a sumar para movernos en la dirección que toque

	// Recorremos todo el tablero comprobando dónde podemos mover
	// Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) { // indica en qué casillas quizá se pueda mover
			if (candidatas[i][j] == SI) {
				if (tablero[i][j] == CASILLA_VACIA) {
					found = 0;
					k = 0;

					// en este bucle comprobamos si es un movimiento válido
					// (es decir si implica voltear en alguna dirección)
					while ((found == 0) && (k < DIM)) {
						SF = vSF[k];    // k representa la dirección que miramos
						SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

						// nos dice qué hay que voltear en cada dirección
						longitud_c_c = longitud_c_arm = longitud_c_thumb =
								longitud_arm_c = longitud_arm_arm =
										longitud_arm_thumb = 0;

						patron_c_c = patron_volteo_c_c(tablero, &longitud_c_c,
								i, j, SF, SC, tipoficha);
						//patron_c_arm = patron_volteo_c_arm(tablero, &longitud_c_arm, i, j, SF, SC, tipoficha);
						//patron_c_thumb = patron_volteo_c_thumb(tablero, &longitud_c_thumb, i, j, SF, SC, tipoficha);
						//patron_arm_c = patron_volteo_arm_c(tablero, &longitud_arm_c, i, j, SF, SC, tipoficha);
						//patron_arm_arm = patron_volteo_arm_arm(tablero, &longitud_arm_arm, i, j, SF, SC, tipoficha);
						//patron_arm_thumb = patron_volteo_arm_thumb(tablero, &longitud_arm_thumb, i, j, SF, SC, tipoficha);
						//  //printf("%d ", patron);
						/*
						 if (patron_c_c == patron_c_arm == patron_c_thumb == patron_arm_c == patron_arm_arm == patron_arm_thumb == PATRON_ENCONTRADO)
						 {*/
						found = 1;
						if (tabla_valor[i][j] > mejor) {
							mf = i;
							mc = j;
							mejor = tabla_valor[i][j];
						}
						/*}else{
						 patron_c_c = patron_volteo_c_c(tablero, &longitud_c_c, i, j, SF, SC, tipoficha);
						 patron_c_arm = patron_volteo_c_arm(tablero, &longitud_c_arm, i, j, SF, SC, tipoficha);
						 patron_c_thumb = patron_volteo_c_thumb(tablero, &longitud_c_thumb, i, j, SF, SC, tipoficha);
						 patron_arm_c = patron_volteo_arm_c(tablero, &longitud_arm_c, i, j, SF, SC, tipoficha);
						 patron_arm_arm = patron_volteo_arm_arm(tablero, &longitud_arm_arm, i, j, SF, SC, tipoficha);
						 patron_arm_thumb = patron_volteo_arm_thumb(tablero, &longitud_arm_thumb, i, j, SF, SC, tipoficha);
						 error_elegirmov();
						 }
						 */
						k++;
						// si no hemos encontrado nada probamos con la siguiente dirección
					}
				}
			}
		}
	}
	*f = (char) mf;
	*c = (char) mc;
	// si no se ha encontrado una posición válida devuelve -1
	return mf;
}
////////////////////////////////////////////////////////////////////////////////
// Cuenta el número de fichas de cada color.
// Los guarda en la dirección b (blancas) y n (negras)
void contar(char tablero[][DIM], int *b, int *n) {
	int i, j;

	*b = 0;
	*n = 0;

	// recorremos todo el tablero contando las fichas de cada color
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			if (tablero[i][j] == FICHA_BLANCA) {
				(*b)++;
			} else if (tablero[i][j] == FICHA_NEGRA) {
				(*n)++;
			}
		}
	}
}

void actualizar_candidatas(char candidatas[][DIM], char f, char c) {
	// donde ya se ha colocado no se puede volver a colocar
	// En las posiciones alrededor sí
	candidatas[f][c] = CASILLA_OCUPADA;
	if (f > 0) {
		if (candidatas[f - 1][c] != CASILLA_OCUPADA)
			candidatas[f - 1][c] = SI;

		if ((c > 0) && (candidatas[f - 1][c - 1] != CASILLA_OCUPADA))
			candidatas[f - 1][c - 1] = SI;

		if ((c < 7) && (candidatas[f - 1][c + 1] != CASILLA_OCUPADA))
			candidatas[f - 1][c + 1] = SI;
	}
	if (f < 7) {
		if (candidatas[f + 1][c] != CASILLA_OCUPADA)
			candidatas[f + 1][c] = SI;

		if ((c > 0) && (candidatas[f + 1][c - 1] != CASILLA_OCUPADA))
			candidatas[f + 1][c - 1] = SI;

		if ((c < 7) && (candidatas[f + 1][c + 1] != CASILLA_OCUPADA))
			candidatas[f + 1][c + 1] = SI;
	}
	if ((c > 0) && (candidatas[f][c - 1] != CASILLA_OCUPADA))
		candidatas[f][c - 1] = SI;

	if ((c < 7) && (candidatas[f][c + 1] != CASILLA_OCUPADA))
		candidatas[f][c + 1] = SI;
}

////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la señal de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deberían ser locales de reversi8,
// la razón es que al meterlas en la pila no las pone juntas, y así jugar es más complicado.
// en esta versión el humano lleva negras y la máquina blancas
// no se comprueba que el humano mueva correctamente.
// Sólo que la máquina realice un movimiento correcto.
extern void genera_exception_dabort();

void reversi8() {

	//pantalla_inicial();

	//genera_exception_dabort();

	////////////////////////////////////////////////////////////////////
	// Tablero candidatas: se usa para no explorar todas las posiciones del tablero
	// sólo se exploran las que están alrededor de las fichas colocadas
	////////////////////////////////////////////////////////////////////
	char __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
			{ { NO, NO, NO, NO, NO, NO, NO, NO }, { NO, NO, NO, NO, NO, NO, NO,
					NO }, { NO, NO, NO, NO, NO, NO, NO, NO }, { NO, NO, NO, NO,
					NO, NO, NO, NO }, { NO, NO, NO, NO, NO, NO, NO, NO }, { NO,
					NO, NO, NO, NO, NO, NO, NO }, { NO, NO, NO, NO, NO, NO, NO,
					NO }, { NO, NO, NO, NO, NO, NO, NO, NO } };

	int done;     // la máquina ha conseguido mover o no
	int move = 0; // el humano ha conseguido mover o no
	int blancas, negras; // número de fichas de cada color
	int fin = 0;  // fin vale 1 si el humano no ha podido mover
				  // (ha introducido un valor de movimiento con algún 8)
				  // y luego la máquina tampoco puede
	char f, c;    // fila y columna elegidas por la máquina para su movimiento

	init_table(tablero, candidatas);
	display_tablero(); ////////////////////////////////////
	timer2_empezar();
	while (fin == 0) {
		int x = timer2_leer();

		move = 0;
		esperar_mov();
		fila = cuenta_fila;
		columna = cuenta_col;
		// si la fila o columna son 8 asumimos que el jugador no puede mover
		if (((fila) != DIM) && ((columna) != DIM)) {
			tablero[fila][columna] = FICHA_NEGRA;
			actualizar_tablero(tablero, fila, columna, FICHA_NEGRA);
			actualizar_candidatas(candidatas, fila, columna);
			move = 1;
		}
		// escribe el movimiento en las variables globales fila columna
		done = elegir_mov(candidatas, tablero, &f, &c, FICHA_BLANCA);
		if (done == -1) {
			if (move == 0)
				fin = 1;
		} else {
			tablero[f][c] = FICHA_BLANCA;
			actualizar_tablero(tablero, f, c, FICHA_BLANCA);
			actualizar_candidatas(candidatas, f, c);
		}
		int y = timer2_leer();
		int res = y - x;

	}
	contar(tablero, &blancas, &negras);
}
