/**
  ******************************************************************************
  * @file           : adm.c
  * @author         : Gonzalo G. Fernandez
  * @brief          : CESE Arquitectura de Microprocesadores
  ******************************************************************************
  * @note
  * Universidad de Buenos Aires
  * Carrera de Especializacion en Sistemas Embebidos
  * Arquitectura de Microprocesadores
  * Resolucion de Ejercicios
  *
  ******************************************************************************
  */

#include "adm.h"
#include "cmsis_gcc.h"


/**
 * @brief Funcion que inicializa un vector con ceros
 * @param   vector: Puntero al vector a inicializar
 * @param   longitud: Longitud del vector a inicializar
*/
static void zeros(uint32_t* vector, uint32_t longitud)
{
    for ( ; longitud > 0; longitud--)
    {
        vector[longitud-1] = 0;
    }
}

/**
 * @brief Funcion que inicializa un vector con ceros
 * @param   vector: Puntero al vector a inicializar
 * @param   longitud: Longitud del vector a inicializar
*/
static void zeros16(uint16_t* vector, uint32_t longitud)
{
    for ( ; longitud > 0; longitud--)
    {
        vector[longitud-1] = 0;
    }
}

/**
 * @brief Funcion que realiza el producto de un vector y un escalar
 * @param   vectorIn: Puntero al vector de entrada
 * @param   vectorOut: Puntero al vector de salida (resultado)
 * @param   longitud: Longitud de los vectores de entrada y salida
 * @param   escalar: Escalar con el que se desar realizar producto
*/
static void productoEscalar32(uint32_t* vectorIn, uint32_t* vectorOut, uint32_t longitud, uint32_t escalar)
{
    for ( ; longitud > 0; longitud--)
    {
        vectorOut[longitud-1] = vectorIn[longitud-1] * escalar;
    }
}

/**
 * @brief Funcion que realiza el producto de un vector y un escalar
 * @param   vectorIn: Puntero al vector de entrada
 * @param   vectorOut: Puntero al vector de salida (resultado)
 * @param   longitud: Longitud de los vectores de entrada y salida
 * @param   escalar: Escalar con el que se desar realizar producto
*/
static void productoEscalar16(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar)
{
    for ( ; longitud > 0; longitud--)
    {
        vectorOut[longitud-1] = vectorIn[longitud-1] * escalar;
    }
}

/**
 * @brief Funcion que realiza el producto de un vector y un escalar saturando el resultado a 12 bits
 * @param   vectorIn: Puntero al vector de entrada
 * @param   vectorOut: Puntero al vector de salida (resultado)
 * @param   longitud: Longitud de los vectores de entrada y salida
 * @param   escalar: Escalar con el que se desar realizar producto
*/
static void productoEscalar12(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar)
{
    for ( ; longitud > 0; longitud--)
    {
        // vectorOut[longitud-1] = (vectorIn[longitud-1] * escalar);
        // vectorOut[longitud-1] = (vectorOut[longitud-1] < 0x0FFF) ? vectorOut[longitud-1] : 0x0FFF;
        vectorOut[longitud-1] = __USAT(vectorIn[longitud-1] * escalar, 12);
    }
}

#define MOVING_AVERAGE_LENGTH 10

/**
 * @brief Funcion que implementa un filtro de ventana movil de 10 valores
 * @param vectorIn: Puntero al vector de entrada
 * @param vectorOut: Puntero al vector de salida (resultado)
 * @param longitud: Longitud de los vectores de entrada y salida
*/
static void filtroVentana10(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud)
{
    uint32_t window_length;
    uint16_t sum;
    for ( ; longitud > 0; longitud--)
    {
        sum = 0;
        window_length = MOVING_AVERAGE_LENGTH;
        if (longitud < MOVING_AVERAGE_LENGTH)
        {
            window_length = longitud;
        }
        while (window_length > 0)
        {
            sum += vectorIn[longitud - window_length];
            window_length--;
        }
        vectorOut[longitud - 1] = (uint16_t)(sum / MOVING_AVERAGE_LENGTH);
    }
}

/**
 * @brief Funcion que empaqueta numeros signados de 32 bits en numeros signados de 16 bits
 * @param vectorIn: Puntero al vector de entrada
 * @param vectorOut: Puntero al vector de salida (resultado)
 * @param longitud: Longitud de los vectores de entrada y salida
*/
static void pack32to16(int32_t* vectorIn, int16_t* vectorOut, uint32_t longitud)
{
    for ( ; longitud > 0; longitud--)
    {
        vectorOut[longitud-1] = (int16_t)__SSAT(vectorIn[longitud-1], 16);
    }
}


/**
 * @brief Funcion que obtiene la posicion del maximo de un vector
 * @param vectorIn: Puntero al vector de entrada
 * @param longitud: Longitud del vector de entrada
*/
static uint32_t max(int32_t* vectorIn, uint32_t longitud)
{
    uint32_t index = longitud - 1;
    longitud--;

    for ( ; longitud > 0; longitud--)
    {
        if (vectorIn[longitud-1] > vectorIn[index])
        {
            index = longitud-1;
        }
    }
    return index;
}

/**
 * @brief Funcion que decima un vector de 32 bits cada N muestras
 * @param vectorIn: Puntero al vector de entrada
 * @param vectorOut: Puntero al vector de salida (resultado)
 * @param longitud: Longitud de los vectores de entrada y salida
 * @param N: Periodo de muestra a descartar
*/
static void downsampleN(int32_t* vectorIn, int32_t* vectorOut, uint32_t longitud, uint32_t N)
{
    uint32_t i, n = 0, m = 0;
    for (i = 0; i < longitud; i++)
    {
        if (m == N - 1)
        {
            m = 0;
            continue;
        }
        vectorOut[n] = vectorIn[i];
        n++;
        m++;
    }
}

/**
 * @brief Funcion que invierte un vector de 16 bits
 * @param vector: Vector a invertir
 * @param longitud: Longitud del vector a invertir
*/
static void invertir (uint16_t* vector, uint32_t longitud)
{
    uint32_t start = 0;
    longitud--;
    uint16_t aux;
    while (start < longitud)
    {
        aux = vector[start];
        vector[start] = vector[longitud];
        vector[longitud] = aux;
        start++;
        longitud--;
    }
}

/* Test variables */
uint32_t arr_len = 5;
uint32_t arr1[] = {0, 1000, 2000, 3000, 4000};
uint16_t arr2[] = {0, 1000, 2000, 3000, 4000};
uint32_t arr3[] = {0, 1001, 2001, 3001, 4001};
uint16_t arr4[] = {0, 1001, 2001, 3001, 4001};
uint16_t ma_input[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
uint16_t ma_output[20];
int32_t pack_input[4] = {-70000, 70000, -500, 500};
int16_t pack_output[4];
int32_t max_input[5] = {-50, 100, 350, -360, 0};
uint32_t max_output;
int32_t downsample_input[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int32_t downsample_output[6];
uint16_t reverse_input[5] = {1, 2, 3, 4, 5};

// #define USE_C
#define USE_ASM

void app_adm(void)
{

#ifdef USE_C
    zeros(arr1, arr_len);
    zeros16(arr2, arr_len);
    productoEscalar32(arr3, arr1, arr_len, 2);
    productoEscalar16(arr4, arr2, arr_len, 3);
    productoEscalar12(arr4, arr2, arr_len, 3);
    filtroVentana10(ma_input, ma_output, 20);
    pack32to16(pack_input, pack_output, 4);
    max_output = max(max_input, 5);
    downsampleN(downsample_input, downsample_output, 9, 3);
    invertir(reverse_input, 5);
#endif /* USE_C */

#ifdef USE_ASM
    asm_zeros(arr1, arr_len);
    asm_zeros16(arr2, arr_len);
    asm_productoEscalar32(arr3, arr1, arr_len, 2);
    asm_productoEscalar16(arr4, arr2, arr_len, 3);
    asm_productoEscalar12(arr4, arr2, arr_len, 3);
    asm_moving_average(ma_input, ma_output, 20);
    asm_pack32to16(pack_input, pack_output, 4);
    max_output = asm_max(max_input, 5);
    asm_downsample(downsample_input, downsample_output, 9, 3);
    asm_reverse(reverse_input, 5);
#endif /* USE_ASM */

    __NOP();
}