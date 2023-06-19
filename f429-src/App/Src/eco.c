/**
 * @brief Función que introduce eco a los 20 ms de un vector muestreado a 44100 muestras/s.
 * El eco consiste en sumar a la muestra la mitad de la amplitud de la muestra a los 20 ms previos.
 * start eco = 44100 muestras/s * 1s/1000ms * 20ms = 882 muestras
*/

#include "eco.h"
#include "cmsis_gcc.h"

// #define ECO_START       882
#define ECO_START       20
// #define INPUT_LENGTH    4096
#define INPUT_LENGTH    80

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


void app_eco(void)
{
    int16_t value = 0;
    for (uint32_t i = 0; i < INPUT_LENGTH; i++)
    {
        value += 5;
        eco_input[i] = value;
    }
    asm_simd_eco(eco_input, INPUT_LENGTH);
    __NOP();
}