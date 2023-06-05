/**
  ******************************************************************************
  * @file           : adm.h
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

#ifndef __ADM_H
#define __ADM_H

#include <stdint.h>

void asm_zeros(uint32_t* vector, uint32_t longitud);
void asm_zeros16(uint16_t* vector, uint16_t longitud);
void asm_productoEscalar32(uint32_t* vectorIn, uint32_t* vectorOut, uint32_t longitud, uint32_t escalar);
void asm_productoEscalar16(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar);
void asm_productoEscalar12(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar);
void app_adm(void);

#endif /* __ADM_H */