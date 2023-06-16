.syntax unified
.thumb
.text

@ Exported symbols
.global asm_corr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ C Prototype
@   void corr(int16_t* vectorX, int16_t* vectorY, int16_t* vectorCorr, uint32_t longitud)
@ Parameters:
@ - r0: vectorX
@ - r1: vectorY
@ - r2: vectorCorr
@ - r3: longitud
@
@ Auxiliar:
@ - r4: l
@ - r5: sum_left
@ - r6: sum_right
@ - r7: i
@ - r8: n
@ - r9: vectorX access, corr_index_left
@ - r10: vectorY access
@
.thumb_func
    asm_corr:
        push {r4-r10}       // save context
        mov r4, #0          // l = 0

    corr_out_loop:
        mov r5, #0          // sum_left = 0
        mov r6, #0          // sum_right = 0
        mov r7, #0          // i = 0
        mov r8, r4          // n = l
    corr_in_loop:
        @ Get left value
        ldrsh r9, [r0, r8, LSL 1]
        ldrsh r10, [r1, r7, LSL 1]
        mla r5, r9, r10, r5

        @ Get right value
        ldrsh r9, [r0, r7, LSL 1]
        ldrsh r10, [r1, r8, LSL 1]
        mla r6, r9, r10, r6

        add r7, #1
        add r8, #1          // update n and check if equal to longitud
        cmp r3, r8
        bne corr_in_loop

        @ Store left operation
        sub r9, r3, r4
        sub r9, #1
        strh r6, [r2, r9, LSL 1]

        # Store right operation
        add r9, r3, r4
        sub r9, #1
        strh r5, [r2, r9, LSL 1]

        add r4, #1          // update l and check if equal to longitud
        cmp r3, r4
        bne corr_out_loop

        pop {r4-r10}         // recover context
        bx lr               // return
