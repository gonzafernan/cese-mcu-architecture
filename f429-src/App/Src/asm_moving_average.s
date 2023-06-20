.syntax unified
.thumb
.text

@ Exported symbols
.global asm_moving_average


#define MA_WINDOW_LENGTH 10

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void asm_moving_average(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud)
@ Parameters:
@ - r0: vectorIn    /input/
@ - r1: vectorOut   /output/
@ - r2: longitud    /length/
@
@ Auxiliar:
@ - r3: sum
@ - r4: window_length
@ - r5: input[i], output[i]
@
.thumb_func
    asm_moving_average:
        push {r4-r5}                // save context

    asm_ma_loop:                    // loop through input
        mov r3, #0                  // sum = 0
        mov r4, MA_WINDOW_LENGTH
        cmp r2, r4
        it le
        movle r4, r2                // window_length = length if length < WINDOW_LENGTH
        push {r4}                   // save window length in stack

    asm_ma_window:                  // loop through window
        sub r5, r2, r4              // length - window_length
        ldrh r5, [r0, r5, LSL 1]    // input[length - window_length]
        add r3, r5                  // sum += input[length - window_length]

        subs r4, #1                 // decrease window left
        bne asm_ma_window           // exit inner loop

        subs r2, #1                 // update output index

        pop {r4}                    // recover window length
        udiv r5, r3, r4             // sum / window_length
        strh r5, [r1, r2, LSL 1]    // output[length-1] = sum / window_length

        bne asm_ma_loop             // exit condition

        pop {r4-r5}                 //recover context
        bx lr                       // return
