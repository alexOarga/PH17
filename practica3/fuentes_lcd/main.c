/*********************************************************************************************
* Fichero:	main.c
* Autor:	
* Descrip:	codigo de entrada de C
* Version:	<P6-ARM>	
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "lcd.h"

/*--- declaracion de funciones ---*/
void main(void);

/*--- funciones externas ---*/
//extern void Lcd_Test();

/*********************************************************************************************
* name:		Lcd_Test()
* func:		LCD test function
* para:		none
* ret:		none
* modify:
* comment:
*********************************************************************************************/
void Lcd_Test(void){
	/* initial LCD controller */
	Lcd_Init();
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();

	/* draw rectangle pattern */
    #ifdef Eng_v // english version
	Lcd_DspAscII8x16(10,0,DARKGRAY,"Embest S3CEV40 ");
	#else
//	Lcd_DspHz16(10,0,DARKGRAY,"英蓓特三星实验评估板");
	#endif
	int TABLERO = 8;
	int CHAR_VER = 16;
	int CHAR_HOR = 8;
	Lcd_DspAscII8x16(0,LCD_YSIZE-CHAR_VER,BLACK,"Pulse o introduzca (8,8)");
	if(LCD_YSIZE < LCD_XSIZE){
		int tamano_casilla = (LCD_YSIZE - CHAR_VER - CHAR_VER)/TABLERO;
		int i = 0;
		for(i=0; i<TABLERO+1; i++){
			if(i < TABLERO){
				char disp[] = {i + '0', 0x0};
				Lcd_DspAscII8x16(0,(tamano_casilla*i)+CHAR_VER,BLACK,disp);
			}
				Lcd_Draw_Line(CHAR_HOR, (tamano_casilla*i)+CHAR_VER, (tamano_casilla*8)+CHAR_HOR, (tamano_casilla*i)+CHAR_VER, 10, 2);
		}
		for(i=0; i<TABLERO+1; i++){
			if(i < TABLERO){
				char disp[] = {i + '0', 0x0};
				Lcd_DspAscII8x16((tamano_casilla*i)+CHAR_HOR, 0,BLACK,disp);
			}
				Lcd_Draw_Line((tamano_casilla*i)+CHAR_HOR,CHAR_VER, (tamano_casilla*i)+CHAR_HOR, (tamano_casilla*8)+CHAR_VER,  10, 2);
		}
	}

	Lcd_Dma_Trans();

}


/*--- codigo de la funcion ---*/
void Main(void)
{
    sys_init();        /* inicializacion de la placa, interrupciones, puertos y UART */
	
	Lcd_Test();        /* inicializacion LCD y visualizacion texto y rectangulos     */
	while(1);
}

