.syntax unified
.thumb
.text

@ Exported symbols
.global asm_zeros
.global asm_zeros16
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_pack32to16
.global asm_max
.global asm_downsample
.global asm_reverse

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
@ Auxiliar:
@ - r4: aux value
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_pack32to16(int32_t* vectorIn, int16_t* vectorOut, uint32_t longitud)
@ Parameters:
@ - r0: vectorIn
@ - r1: vectorOut
@ - r2: longitud
@
@ Auxiliar:
@ - r3: value
@
.thumb_func
    asm_pack32to16:
        subs r2, #1                 // update index

        ldr r3, [r0, r2, LSL 2]     // load input value
        ssat r3, #16, r3            // saturate value to 16 bits
        strh r3, [r1, r2, LSL 1]    // save output value

        bne asm_pack32to16          // exit condition
        bx lr ;                     // return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@ uint32_t asm_max(int32_t* vectorIn, uint32_t longitud)
@ Parameters:
@ - r0: vectorIn        /input/
@ - r1: longitud        /length/
@
@ Auxiliar:
@ - r2: index
@ - r3: max value
@ - r4: value to compare
@
.thumb_func
    asm_max:
        push {r4}                   // save context
        sub r1, #1 ;                // length - 1
        mov r2, r1 ;                // index = length - 1
        ldr r3, [r0, r2, LSL 2]     // max value initialization
    asm_max_loop:
        ldr r4, [r0, r1, LSL 2] ;   // value = vectorIn[i]
        cmp r3, r4
        itt lt                       // compare with max value
        movlt r3, r4                // update max value
        movlt r2, r1                // update max index

        subs r1, #1                 // update length
        bne asm_max_loop            // exit condition

        mov r0, r2                  // update return value
        push {r4}                   // recover context
        bx lr ;                     // Return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@ void downsampleN(int32_t* vectorIn, int32_t* vectorOut, uint32_t longitud, uint32_t N)
@ Parameters:
@ - r0: vectorIn        /input/
@ - r1: vectorOut       /output/
@ - r2: longitud        /length/
@ - r3: N
@
@ Auxiliar:
@ - r4: input index
@ - r5: input/output value
@ - r6: downsample counter
@
.thumb_func
    asm_downsample:
        push {r4-r6}                // save context
        mov r4, #0                  // aux values in 0
        mov r5, #0
        mov r6, #0
        sub r3, #1                  // use N-1 due to index 0

    asm_downsample_loop:            // loop through input
        ldr r5, [r0, r4, LSL 2]     // load input value
        cmp r6, r3                  // check for sample N
        itee eq
        moveq r6, #0
        strne r5, [r1], 4
        addne r6, #1

        add r4, #1                  // update input index
        cmp r4, r2                  // exit condition
        bne asm_downsample_loop

        pop {r4-r6}                 // pop context
        bx lr ;                     // Return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_reverse (uint16_t* vector, uint32_t longitud)
@ Parameters:
@ - r0: vector
@ - r1: longitud
@
@ Auxiliar:
@ - r2: start
@ - r3: left value
@ - r4: right value
@
.thumb_func
    asm_reverse:
        push {r4}                   // save context
        mov r2, #0
        sub r1, #1

    asm_reverse_loop:
        ldrh r3, [r0, r2, LSL 1]    // load left value
        ldrh r4, [r0, r1, LSL 1]    // load right value
        strh r3, [r0, r1, LSL 1]    // exchange value
        strh r4, [r0, r2, LSL 1]

        sub r1, #1                  // uptade index
        add r2, #1

        cmp r2, r1                  // exit condition
        ble asm_reverse_loop

        pop {r4}
        bx lr ;                     // Return
