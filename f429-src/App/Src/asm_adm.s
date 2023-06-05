.syntax unified
.thumb
.text

@ Exported symbols
.global asm_zeros
.global asm_zeros16
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_zeros(uint32_t* vector, uint32_t longitud)
@ Parameters:
@ - r0: vector
@ - r1: longitud
@
.thumb_func
    asm_zeros:
        mov r2, #0 ;
    zeros_loop:
        subs r1, #1 ;               // Decrementar longitud con update de flags
        str r2, [r0, r1, LSL 2] ;
        bne zeros_loop ;            // str no modifica los flags de la ALU
        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_zeros16(uint16_t* vector, uint32_t longitud)
@ Parameters:
@ - r0: vector
@ - r1: longitud
@
.thumb_func
    asm_zeros16:
        mov r2, #0 ;
    zeros16_loop:
        subs r1, #1 ;
        str r2, [r0, r1, LSL 1] ;
        bne zeros16_loop ;
        bx lr ;

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_productoEscalar32(uint32_t* vectorIn, uint32_t* vectorOut, uint32_t longitud, uint32_t escalar)
@ Parameters:
@ - r0: vectorIn
@ - r1: vectorOut
@ - r2: longitud
@ - r3: escalar
@   
.thumb_func
    asm_productoEscalar32:
        push {r4} ;                 // Save context in stack
    mult32_loop:
        subs r2, #1 ;               // Update index
        ldr r4, [r0, r2, LSL 2] ;   // r4 = vectorIn[i]
        mul r4, r4, r3 ;            // Multiply: r4 = vectorIn[i] * escalar
        str r4, [r1, r2, LSL 2] ;   // Save result: vectorOut[i] = r4 
        bne mult32_loop ;           // Loop if index not 0
        pop {r4} ;                  // Recover context
        bx lr ;                     // Return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_productoEscalar16(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar)
@ Parameters:
@ - r0: vectorIn
@ - r1: vectorOut
@ - r2: longitud
@ - r3: escalar
@   
.thumb_func
    asm_productoEscalar16:
        push {r4} ;                 // Save context in stack
    mult16_loop:
        subs r2, #1 ;               // Update index
        ldrh r4, [r0, r2, LSL 1] ;  // r4 = vectorIn[i]
        mul r4, r4, r3 ;            // Multiply: r4 = vectorIn[i] * escalar
        strh r4, [r1, r2, LSL 1] ;  // Save result: vectorOut[i] = r4 
        bne mult16_loop ;           // Loop if index not 0
        pop {r4} ;                  // Recover context
        bx lr ;                     // Return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_productoEscalar12(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar)
@ Parameters:
@ - r0: vectorIn
@ - r1: vectorOut
@ - r2: longitud
@ - r3: escalar
@   
.thumb_func
    asm_productoEscalar12:
        push {r4} ;                 // Save context in stack
    mult12_loop:
        subs r2, #1 ;               // Update index
        ldrh r4, [r0, r2, LSL 1] ;  // r4 = vectorIn[i]
        mul r4, r4, r3 ;            // Multiply: r4 = vectorIn[i] * escalar
        usat r4, 12, r4 ;           // Saturation 12
        strh r4, [r1, r2, LSL 1] ;  // Save result: vectorOut[i] = r4 
        bne mult12_loop ;           // Loop if index not 0
        pop {r4} ;                  // Recover context
        bx lr ;                     // Return
