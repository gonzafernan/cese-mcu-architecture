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
    for (longitud; longitud > 0; longitud--)
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
    for (longitud; longitud > 0; longitud--)
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
    for (longitud; longitud > 0; longitud--)
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
    for (longitud; longitud > 0; longitud--)
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
    for (longitud; longitud > 0; longitud--)
    {
        // vectorOut[longitud-1] = (vectorIn[longitud-1] * escalar);
        // vectorOut[longitud-1] = (vectorOut[longitud-1] < 0x0FFF) ? vectorOut[longitud-1] : 0x0FFF;
        vectorOut[longitud-1] = __USAT(vectorIn[longitud-1] * escalar, 12);
    }
    
}

void app_adm(void)
{
    uint32_t arr_len = 5;
    uint32_t arr1[] = {0, 1000, 2000, 3000, 4000};
    uint16_t arr2[] = {0, 1000, 2000, 3000, 4000};
    uint32_t arr3[] = {0, 1001, 2001, 3001, 4001};
    uint16_t arr4[] = {0, 1001, 2001, 3001, 4001};

//#define USE_C
#define USE_ASM
#ifdef USE_C
    zeros(arr1, arr_len);
    zeros16(arr2, arr_len);
    productoEscalar32(arr3, arr1, arr_len, 2);
    productoEscalar16(arr4, arr2, arr_len, 3);
    productoEscalar12(arr4, arr2, arr_len, 3);
#endif /* USE_C */

#ifdef USE_ASM
    asm_zeros(arr1, arr_len);
    asm_zeros16(arr2, arr_len);
    asm_productoEscalar32(arr3, arr1, arr_len, 2);
    asm_productoEscalar16(arr4, arr2, arr_len, 3);
    asm_productoEscalar12(arr4, arr2, arr_len, 3);
#endif /* USE_ASM */
    __NOP();
}