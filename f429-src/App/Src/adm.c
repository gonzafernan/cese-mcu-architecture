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

/**
 * @brief Funcion que inicializa un vector con ceros
 * @param   vector: Puntero al vector a inicializar
 * @param   longitud: Longitud del vector a inicializar
*/
static void zeros(uint32_t* vector, uint32_t longitud)
{
    for (uint32_t i = 0; i < longitud; i++)
    {
        vector[i] = 0;
    }
}

/**
 * @brief Funcion que inicializa un vector con ceros
 * @param   vector: Puntero al vector a inicializar
 * @param   longitud: Longitud del vector a inicializar
*/
static void zeros16(uint16_t* vector, uint32_t longitud)
{
    for (uint32_t i = 0; i < longitud; i++)
    {
        vector[i] = 0;
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
    for (uint32_t i = 0; i < longitud; i++)
    {
        vectorOut[i] = vectorIn[i] * escalar;
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
    for (uint32_t i = 0; i < longitud; i++)
    {
        vectorOut[i] = vectorIn[i] * escalar;
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
    for (uint32_t i = 0; i < longitud; i++)
    {
        vectorOut[i] = (vectorIn[i] * escalar);
        vectorOut[i] = (vectorOut[i] < 0x0FFF) ? vectorOut[i] : 0x0FFF;
    }
    
}

void app_adm(void)
{
    uint32_t arr_len = 5;
    uint16_t arr1[] = {0, 1000, 2000, 3000, 4000};
    uint16_t arr2[arr_len];

    zeros16(arr2, arr_len);
    productoEscalar12(arr1, arr2, arr_len, 2);
}