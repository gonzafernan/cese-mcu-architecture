.syntax unified
.thumb
.text

@ Exported symbols
.global asm_eco
.global asm_simd_eco

#define ECO_START       20
#define ECO_EXIT_SIMD   ECO_START - 2   // Otherwise, It'll skip the last iteration

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_eco(int16_t* signal, uint32_t length)
@ Parameters:
@ - r0: signal
@ - r1: length
@
@ Auxiliar:
@ - r2: eco index
@ - r3: input value
@ - r4: eco value
@
.thumb_func
    asm_eco:
        push {r4}                   // save context
        sub r2, r1, ECO_START       // init eco index
        sub r1, #1

    asm_eco_loop:
        ldrsh r3, [r0, r1, LSL 1]   // input[i]; i -= 2;
        ldrsh r4, [r0, r2, LSL 1]   // eco[i] = input[i - ECO_START]
        asr r4, #1                  // eco[i] = input[i - ECO_START] / 2
        add r3, r4                  // add eco
        strh r3, [r0, r1, LSL 1]    // store result
        
        sub r1, #1
        sub r2, #1
        cmp r1, ECO_START
        bne asm_eco_loop

        pop {r4}                    // recover context
        bx lr                       // return

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_simd_eco(int16_t* signal, uint32_t length)
@ Parameters:
@ - r0: signal
@ - r1: length
@
@ Auxiliar:
@ - r2: eco index
@ - r3: input value
@ - r4: eco value 1
@ - r5: eco value 2
@
.thumb_func
    asm_simd_eco:
        push {r4-r5}                // save context
        sub r1, #2
        sub r2, r1, ECO_START       // init eco index

    asm_simd_eco_loop:
        ldr r3, [r0, r1, LSL 1]     // load input[i] and input[i+1];

        ldrsh r4, [r0, r2, LSL 1]   // eco[i] = input[i-ECO_START]
        asr r4, r4, #1              // eco[i] = eco[i]/2
        add r2, #1

        ldrsh r5, [r0, r2, LSL 1]   // eco[i+1] = input[i-ECO_START+1]
        asr r5, r5, #1                  // eco[i+1] = eco[i+1]/2
        lsl r5, r5, #16             // put eco[i] in MSB
        orr r4, r4, r5     // pack 2 int16_t in the 32 bit register

        qadd16 r3, r3, r4           // simd add pair
        str r3, [r0, r1, LSL 1]    // store result
        
        sub r1, #2
        sub r2, #3
        cmp r1, ECO_EXIT_SIMD
        bne asm_simd_eco_loop

        pop {r4-r5}                 // recover context
        bx lr                       // return