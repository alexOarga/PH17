.text
#        ENTRY            		/*  mark the first instruction to call */
.global	start
.global ficha_valida_arm
.global patron_volteo_arm_c
.global patron_volteo_arm_arm
.global patron_volteo_arm_thumb
.global ficha_valida_thumb
.extern ficha_valida

start:
.arm /*indicates that we are using the ARM instruction set */
#------standard initial code
# --- Setup interrupt / exception vectors 
      B       Reset_Handler 
/* In this version we do not use the following handlers */
#######################################################################################################
#-----------Undefined_Handler: 
#      B       Undefined_Handler 
#----------SWI_Handler: 
#      B       SWI_Handler 
#----------Prefetch_Handler: 
#      B       Prefetch_Handler 
#----------Abort_Handler: 
#      B       Abort_Handler 
#         NOP      /* Reserved vector */ 
#----------IRQ_Handler: 
#      B       IRQ_Handler 
#----------FIQ_Handler: 
#      B       FIQ_Handler 
#######################################################################################################
# Reset Handler:
# the processor starts executing this code after system reset       
#######################################################################################################
Reset_Handler:  
#        
        MOV     sp, #0x4000      /*  set up stack pointer (r13) */ 

.extern     reversi8
        ldr         r5, = reversi8
        mov         lr, pc 
        bx          r5 


stop: 	
 		B     	stop    	/*  end of program */

######################################################################################################
//ficha_valida_arm:
//	Parametros:
// 		r0= tablero,  r1= fila,  r2= columna,  r3 = dir(posicion_valida)
//	Resultado:
//		Devuelve ficha en r0. Escribe 1 o 0 en posicion_valida
######################################################################################################
ficha_valida_arm:

	; r0= tablero
	; r1= fila
	; r2= columna
	; r3 = posicion_valida
	    cmp     r2, #7					// se compara unsigned si son >7
        cmpls   r1, #7
        bhi     .else_fv_arm			// salta a else si es mayor o negativo
        add     r0, r0, r1, lsl #3		// multiplica por 8 (DIM)
        ldrb    r0, [r0, r2]			// carga tablero[f][c]
        cmp     r0, #0
        bne     .if_fv_arm				// si no es igual entra a if
.else_fv_arm:
        str     r0, [r3, #0]			// r0 es 0(ficha), se escribe posicion valida. Se deuvelve 0.
        bx      r14
.if_fv_arm:
        mov     r0, #1					// r0 se devuelve (contiene ficha). Se escribe 1 en posicion valida.
        str     r0, [r3, #0]
        bx      r14
#################################################################################################################
// patron_volteo_arm_arm:
// parametros: r0=tablero, r1=dir(longitud), r2=FA, r3=CA
//				En pila: SF, SC, color
// devuelve: En r0: 1 o 0 dependiendo de la posicion (FA,CA), el color y la longitud
//			 Aumenta longitud en 1 o mas unidades(mediante llamadas recurisvas) si la
//			 posicion de la ficha es valida y no es igual a color
######################################################################################################
patron_volteo_arm_arm:

	MOV	ip, sp
	STMDB	sp!, {r4-r9, fp, ip, lr, pc}
	SUB	fp, ip, #4
	//espacio variable local
	SUB sp, sp, #4

	//se guarda tablero y longitud para invocar a ficha_valida()
	MOV r7, r1
	MOV r8, r0

	//saca los parametros SF SC de la pila
	LDRB r4, [fp, #4]
	LDRB r5, [fp, #8]

	//ADD y llamada ficha valida
	ADD r1, r2, r4
	AND r1, r1, #255
	ADD r2, r3, r5
	AND r2, r2, #255
	SUB r3, fp, #32
	MOV r6, r1				//guarda fa y fc
	MOV r9, r2
	BL ficha_valida_arm
	//r0 = casilla, r1 = dir(posicion)
	//r7 = dir(longitud) ,r8 = tablero
	MOV r3, r9				//restarura parametros
	MOV r2, r6
	MOV r1, r7
	LDR r6, [fp, #-32]			//posicion se utiliza luegi
	//LDR r6, =Pos_valida
	//LDR r6, [r6]
	//r0 = casilla, r1 = dir(longitud), r2 = FA, r3=C, r4=SF, r5=SC
	//r6 = **, r7 = dir(longitud) ,r8 = tablero
					// IF poscion valida = 1
	CMP r6, #1
	BNE else_pv
	LDRB r6, [fp, #12]			// cargamos color de la pila
	LDR  r7, [r1]				// se carga *longitud (se utiliza despues)
	CMP r6, r0					// IF color==casilla
	BEQ elseif_pv
	//r0 = casilla, r1 = dir(longitud), r2 = FA, r3=C, r4=SF, r5=SC
	//r6 = color, r7 = longitud ,r8 = tablero

// if{
	ADD r7, r7, #1
	STR r7, [r1]
	MOV r0, r8				// tablero a r0 para llamar a patron_volteo
	STMFD   sp!, {r4-r6}	// apilamos sf, sc, color
	BL patron_volteo_arm_arm	// patron_voleto()
	ADD sp, sp, #12			// desapilamos los parametros
	LDMDB	fp, {r4-r9, fp, sp, pc}

elseif_pv:
    CMP     r7, #0			// devuelve el resulado segun el valor de longitud
    MOVGT   r0, #1
    MOVLE   r0, #0
    LDMDB	fp, {r4-r9, fp, sp, pc}


else_pv:					// else
	MOV r0, #0				// devuelve 0
	LDMDB	fp, {r4-r9, fp, sp, pc}

#################################################################################################################
// patron_volteo_arm_c:
// parametros: r0=tablero, r1=dir(longitud), r2=FA, r3=CA
//				En pila: SF, SC, color
// devuelve: En r0: 1 o 0 dependiendo de la posicion (FA,CA), el color y la longitud
//			 Aumenta longitud en 1 o mas unidades(mediante llamadas recurisvas) si la
//			 posicion de la ficha es valida y no es igual a color
######################################################################################################
patron_volteo_arm_c:

	MOV	ip, sp
	STMDB	sp!, {r4-r9, fp, ip, lr, pc}
	SUB	fp, ip, #4
	//espacio variable local
	SUB sp, sp, #4

	//se guarda tablero y longitud para invocar a ficha_valida()
	MOV r7, r1
	MOV r8, r0

	//saca los parametros SF SC de la pila
	LDRB r4, [fp, #4]
	LDRB r5, [fp, #8]

	ADD r1, r2, r4
	AND r1, r1, #255
	ADD r2, r3, r5
	AND r2, r2, #255
	SUB r3, fp, #32
	MOV r6, r1				//guarda fa y fc
	MOV r9, r2
	BL ficha_valida
	//r0 = casilla, r1 = dir(posicion)
	//r7 = dir(longitud) ,r8 = tablero
	//r0 = casilla, r1 = dir(posicion)
	//r7 = dir(longitud) ,r8 = tablero
	MOV r3, r9				//restarura parametros
	MOV r2, r6
	MOV r1, r7
	LDR r6, [fp, #-32]			//posicion se utiliza luegi
	//LDR r6, =Pos_valida
	//LDR r6, [r6]
	//r0 = casilla, r1 = dir(longitud), r2 = FA, r3=C, r4=SF, r5=SC
	//r6 = **, r7 = dir(longitud) ,r8 = tablero
					// IF poscion valida = 1
	CMP r6, #1
	BNE else_pv
	LDRB r6, [fp, #12]			// cargamos color de la pila
	LDR  r7, [r1]				// se carga *longitud (se utiliza despues)
	CMP r6, r0					// IF color==casilla
	BEQ elseif_pv
	//r0 = casilla, r1 = dir(longitud), r2 = FA, r3=C, r4=SF, r5=SC
	//r6 = color, r7 = longitud ,r8 = tablero

// if{
	ADD r7, r7, #1
	STR r7, [r1]
	MOV r0, r8				// tablero a r0 para llamar a patron_volteo
	STMFD   sp!, {r4-r6}	// apilamos sf, sc, color
	BL patron_volteo_arm_c	// patron_voleto()
	ADD sp, sp, #12			// desapilamos los parametros
	LDMDB	fp, {r4-r9, fp, sp, pc}
elseif_pv_1:
    CMP     r7, #0			// devuelve el resulado segun el valor de longitud
    MOVGT   r0, #1
    MOVLE   r0, #0
	LDMDB	fp, {r4-r9, fp, sp, pc}

else_pv_1:					// else
	MOV r0, #0				// devuelve 0
	LDMDB	fp, {r4-r9, fp, sp, pc}
#################################################################################################################
// patron_volteo_arm_thumb:
// parametros: r0=tablero, r1=dir(longitud), r2=FA, r3=CA
//				En pila: SF, SC, color
// devuelve: En r0: 1 o 0 dependiendo de la posicion (FA,CA), el color y la longitud
//			 Aumenta longitud en 1 o mas unidades(mediante llamadas recurisvas) si la
//			 posicion de la ficha es valida y no es igual a color
######################################################################################################
patron_volteo_arm_thumb:

	MOV	ip, sp
	STMDB	sp!, {r4-r9, fp, ip, lr, pc}
	SUB	fp, ip, #4
	//espacio variable local
	SUB sp, sp, #4

	//se guarda tablero y longitud para invocar a ficha_valida()
	MOV r7, r1
	MOV r8, r0

	//saca los parametros SF SC de la pila
	LDRB r4, [fp, #4]
	LDRB r5, [fp, #8]

	//ADD y llamada ficha valida
	ADD r1, r2, r4
	AND r1, r1, #255
	ADD r2, r3, r5
	AND r2, r2, #255
	SUB r3, fp, #32
	MOV r6, r1				//guarda fa y fc
	MOV r9, r2
	BL ficha_valida_thumb
	//r0 = casilla, r1 = dir(posicion)
	//r7 = dir(longitud) ,r8 = tablero
	MOV r3, r9				//restarura parametros
	MOV r2, r6
	MOV r1, r7
	LDR r6, [fp, #-32]			//posicion se utiliza luegi
	//LDR r6, =Pos_valida
	//LDR r6, [r6]
	//r0 = casilla, r1 = dir(longitud), r2 = FA, r3=C, r4=SF, r5=SC
	//r6 = **, r7 = dir(longitud) ,r8 = tablero
					// IF poscion valida = 1
	CMP r6, #1
	BNE else_pv
	LDRB r6, [fp, #12]			// cargamos color de la pila
	LDR  r7, [r1]				// se carga *longitud (se utiliza despues)
	CMP r6, r0					// IF color==casilla
	BEQ elseif_pv
	//r0 = casilla, r1 = dir(longitud), r2 = FA, r3=C, r4=SF, r5=SC
	//r6 = color, r7 = longitud ,r8 = tablero

// if{
	ADD r7, r7, #1
	STR r7, [r1]
	MOV r0, r8				// tablero a r0 para llamar a patron_volteo
	STMFD   sp!, {r4-r6}	// apilamos sf, sc, color
	BL patron_volteo_arm_thumb	// patron_voleto()
	ADD sp, sp, #12			// desapilamos los parametros
	LDMDB	fp, {r4-r9, fp, sp, pc}
elseif_pv_2:
    CMP     r7, #0			// devuelve el resulado segun el valor de longitud
    MOVGT   r0, #1
    MOVLE   r0, #0
	LDMDB	fp, {r4-r9, fp, sp, pc}

else_pv_2:					// else
	MOV r0, #0				// devuelve 0
	LDMDB	fp, {r4-r9, fp, sp, pc}
#################################################################################################################


######################################################################################################
//ficha_valida_thumb:
//	Parametros:
// 		r0= tablero,  r1= fila,  r2= columna,  r3 = dir(posicion_valida)
//	Resultado:
//		Devuelve ficha en r0. Escribe 1 o 0 en posicion_valida
######################################################################################################
.thumb_func
ficha_valida_thumb:
        push    {r4}
        mov     r4, #7
        cmp     r4, r2			// IF c > DIM
        bcc     .Elsethumb
        cmp     r4, r1			// IF f > DIM
        bcc     .Elsethumb
        lsl     r4, r1, #3		// f * 8(DIM)
        add     r0, r0, r4		// tablero + r4
        ldrb    r4, [r0, r2]	// r4 = tablero + (f*8) + c
        cmp     r4, #0			// IF CASILLA_VACIA
        beq     .Elsethumb
// if{
        mov     r0, #1
        str     r0, [r3]
        mov		r0, r4
        pop     {r4}
        bx		r14
.Elsethumb:
        str     r4, [r3]		//r4 tiene 0
        mov     r0, #0
        pop     {r4}
        bx		r14
#################################################################################################################

#################################################################################################################

.data
.equ DIM, 8
.equ CASILLA_VACIA, 0
.equ CASILLA_LLENA, 1
Pos_valida:	.space 4

.end
#        END
