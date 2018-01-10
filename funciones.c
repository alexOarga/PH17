volatile int blancas, negras; // número de fichas de cada color
volatile int fin = 0;  // fin vale 1 si el humano no ha podido mover

if(pulsa_en_fin(puls_x, puls_y)){
	fin = 1;
}

pantalla_final();

void pantalla_final(){
		volatile INT8U* fin = "Fin de la partida";
		volatile INT8U* fiblancas = "Fichas blancas:";
		volatile INT8U* finegras = "Fichas negras:";
		volatile INT8U* numblancas = itoa(blancas);
		volatile INT8U* numblancas = itoa(negras);
		int inicio_y = (LCD_YSIZE/2);
		int inicio_x = (LCD_XSIZE / 2) - (CHAR_HOR * 13);
		Lcd_DspAscII8x16(inicio_x, inicio_y, BLACK,
			fin);
		Lcd_DspAscII8x16(inicio_x, inicio_y + CHAR_VER*2, BLACK,
			fiblancas);
		Lcd_DspAscII8x16(inicio_x + (CHAR_HOR*15), inicio_y + CHAR_VER*2, BLACK,
			numblancas);
		Lcd_DspAscII8x16(inicio_x, inicio_y + CHAR_VER*3, BLACK,
			fiblancas);
		Lcd_DspAscII8x16(inicio_x + (CHAR_HOR*15), inicio_y + CHAR_VER*3, BLACK,
			numblancas);
}