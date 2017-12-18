/*********************************************************************************************
* File£º	tp.c
* Author:	embest	
* Desc£º	LCD touch screen control function
* History:	
*********************************************************************************************/

/*--- include files ---*/

#include "tp.h"
#include "lcd.h"
#include "def.h"

int ultima_pulsacion(void){
	return contador_pulsaciones;	
}

int pulsacion_X_CORD(void){
	return pulsacion_x;	
}

int pulsacion_Y_CORD(void){
	return pulsacion_y;	
}

void TS_Test(void)
{
		Lcd_TC();
		TS_init();
		Check_Sel();
			
		//Uart_Printf("\n Pixel: 320 X 240. Coordinate Rang in: (0,0) - (320,240)\n");
		//Uart_Printf("\nLCD TouchScreen Test Example(please touch LCD screen)\n");
}
/*--- function code ---*/
/*********************************************************************************************
* name:		TSInt
* func:		TouchScreen interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void TSInt(void)
{

    

    int   i;
    char fail = 0;
    ULONG tmp;
    ULONG Pt[6];

	// <X-Position Read>
	// TSPX(GPE4_Q4(+)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(+)) TSMX(GPE7_Q1(-))
    //       0               1                 1                0
	rPDATE=0x68;
	rADCCON=0x1<<2;			// AIN1
	
	Delay(500);                // delay to set up the next channel
	for( i=0; i<5; i++ )
	{
		rADCCON |= 0x1;				// Start X-position A/D conversion
	    while( rADCCON & 0x1 );		// Check if Enable_start is low
    	while( !(rADCCON & 0x40) );	// Check ECFLG
	    Pt[i] = (0x3ff&rADCDAT);
	}
	// read X-position average value
	Pt[5] = (Pt[0]+Pt[1]+Pt[2]+Pt[3]+Pt[4])/5;
	
	tmp = Pt[5];	
	
    // <Y-Position Read>
	// TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(+)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+))
    //       1               0                 0                1
	rPDATE=0x98;
	rADCCON=0x0<<2;		        	// AIN0
	
	Delay(500);                // delay to set up the next channel
	for( i=0; i<5; i++ )
	{
    	rADCCON |= 0x1;             // Start Y-position conversion
	    while( rADCCON & 0x1 );     // Check if Enable_start is low
    	while( !(rADCCON & 0x40) ); // Check ECFLG
	    Pt[i] = (0x3ff&rADCDAT);
	}
	// read Y-position average value
	Pt[5] = (Pt[0]+Pt[1]+Pt[2]+Pt[3]+Pt[4])/5;

	if(!(CheckTSP|(tmp < Xmin)|(tmp > Xmax)|(Pt[5] < Ymin)|(Pt[5] > Ymax)))   // Is valid value?
	  {
		tmp = 320*(tmp - Xmin)/(Xmax - Xmin);   // X - position

			
		Pt[5] = 240*(Pt[5] - Xmin)/(Ymax - Ymin);

      }

	// asignamos ultima posicion //////////////////////////////////
	pulsacion_x = tmp;
	pulsacion_y = Pt[5];
	contador_pulsaciones++;

    if(CheckTSP)
 	/*----------- check to ensure Xmax Ymax Xmin Ymin ------------*/
 	    DesignREC(tmp,Pt[5]);

	rPDATE = 0xb8;                  // should be enabled	
	Delay(6000);                // delay to set up the next channel

    rI_ISPC = BIT_EINT2;            // clear pending_bit
}
			
/*********************************************************************************************
* name:		TS_init
* func:		initialize TouchScreen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void TS_init(void)
{	
    /* enable interrupt */

    oneTouch = 0;

    // inicializamos contador de pulsaciones //

    contador_pulsaciones = 0;

    // inic ultima pulsacion //

    pulsacion_x = 0;
    pulsacion_y = 0;

	rINTMOD=0x0;
	rINTCON=0x1;
    rI_ISPC |= BIT_EINT2;            // clear pending_bit
	
	// TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+)) 
	//          1               1                0                 1
    rPUPE  = 0x0;	                 // Pull up
    rPDATE = 0xb8;                   // should be enabled	
    Delay(100);
    
    rEXTINT |= 0x200;                // falling edge trigger
    pISR_EINT2=(int)user_irq1;       // set interrupt handler
    
    rCLKCON = 0x7ff8;                // enable clock
    rADCPSR = 0x1;//0x4;             // A/D prescaler
    rINTMSK &= ~(BIT_GLOBAL|BIT_EINT2);
}

/*********************************************************************************************
* name:		TS_close
* func:		close TouchScreen
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void TS_close(void)
{
	/* Mask interrupt */
	rINTMSK |= (BIT_GLOBAL|BIT_EINT2);
	pISR_EINT2 = (int)NULL;
}

void Lcd_TC(void)
{
	/* initial LCD controller */
	Lcd_Init();
	/* clear screen */
	Lcd_Clr();
	Lcd_Active_Clr();
	/* draw rectangle pattern */ 
	Lcd_Draw_Box(0,0,80,60,15);
	Lcd_Draw_Box(80,0,160,60,15);
	Lcd_Draw_Box(160,0,240,60,15);
	Lcd_Draw_Box(240,0,320,60,15);
	Lcd_Draw_Box(0,60,80,120,15);
	Lcd_Draw_Box(80,60,160,120,15);
	Lcd_Draw_Box(160,60,240,120,15);
	Lcd_Draw_Box(240,60,320,120,15);
	Lcd_Draw_Box(0,120,80,180,15);
	Lcd_Draw_Box(80,120,160,180,15);
	Lcd_Draw_Box(160,120,240,180,15);
	Lcd_Draw_Box(240,120,320,180,15);
	Lcd_Draw_Box(0,180,80,240,15);
	Lcd_Draw_Box(80,180,160,240,15);
	Lcd_Draw_Box(160,180,240,240,15);
	Lcd_Draw_Box(240,180,320,240,15);
	/* output ASCII symbol */
	Lcd_Dma_Trans();
	Delay(100);
}

/*********************************************************************************************
* name:		DesignREC
* func:		confirm the coordinate rang
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void DesignREC(ULONG tx, ULONG ty)
{
    int tm;
    

    if(oneTouch == 0)
     {
       Vx = tx;                   // Vx as Xmax
       Vy = ty;                   // Vy as Ymax
       oneTouch = 1;       		           
    }else{
    
    if(Vx < tx )
     {
       tm = tx; tx = Vx; Vx = tm; // tx as Xmin
     }
    if(Vy < ty )
     { 
       tm = ty; ty = Vy; Vy = tm; // ty as Ymin
     }
     
    Xmax = Vx;    Xmin = tx;
    Ymax = Vy;    Ymin = ty;
    
    oneTouch = 0;
	CheckTSP = 0;// has checked
	
	}// end if(oneTouch == 0)

}

void Check_Sel(void)
{
	char yn;
    
	do{
	  
	  rINTMSK |=BIT_GLOBAL|BIT_EINT2;
      rI_ISPC = BIT_EINT2;              // clear pending_bit
      rINTMSK =~(BIT_GLOBAL|BIT_EINT2);

         //Init X Y rectangle
          Xmax = 750;    Xmin = 200;
          Ymax = 620;    Ymin = 120;

	      oneTouch = 0;
	      CheckTSP = 1; // mask to check
	      while(CheckTSP);
	      
	   // }else break;
	  
	    }while(1);
}
