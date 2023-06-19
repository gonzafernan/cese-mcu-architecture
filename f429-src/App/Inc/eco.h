/**
  ******************************************************************************
  * @file           : eco.h
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

#ifndef __ECO_H
#define __ECO_H

#include <stdint.h>

void asm_eco(int16_t* signal, uint32_t length);
void asm_simd_eco(int16_t* signal, uint32_t length);
void app_eco(void);

#endif /* __ECO_H */