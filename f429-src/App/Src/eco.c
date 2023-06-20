/**
 * @brief Función que introduce eco a los 20 ms de un vector muestreado a 44100 muestras/s.
 * El eco consiste en sumar a la muestra la mitad de la amplitud de la muestra a los 20 ms previos.
 * start eco = 44100 muestras/s * 1s/1000ms * 20ms = 882 muestras
*/

#include "perf_counter.h"
#include "eco.h"
#include "cmsis_gcc.h"

#define ECO_START       882
// #define ECO_START       20
#define INPUT_LENGTH    4096
// #define INPUT_LENGTH    80

int16_t eco_input[INPUT_LENGTH];

/**
 * @brief Función que introduce eco a los 20ms en un vector muestreado a 44100 muestras/s
 * @param signal: puntero al vector de entrada
 * @param length: longitud del vector de entrada
*/
static void eco(int16_t* signal, uint32_t length)
{
    for ( ; length > ECO_START; length--)
    {
        signal[length-1] += signal[length - ECO_START - 1] >> 1;
    }
}

uint32_t eco_counter[3];

void app_eco(void)
{
    int16_t value = 0;
    for (uint32_t i = 0; i < INPUT_LENGTH; i++)
    {
        value += 5;
        eco_input[i] = value;
    }
    /* Eco C */
    perf_counter_reset();
    eco(eco_input, INPUT_LENGTH);
    eco_counter[0] = perf_counter_get();

    /* Eco ASM */
    perf_counter_reset();
    asm_eco(eco_input, INPUT_LENGTH);
    eco_counter[1] = perf_counter_get();

    /* Eco ASM with SIMD */
    perf_counter_reset();
    asm_simd_eco(eco_input, INPUT_LENGTH);
    eco_counter[2] = perf_counter_get();

    __NOP();
}