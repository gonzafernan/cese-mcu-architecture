/**
  ******************************************************************************
  * @file           : corr.h
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

#ifndef __CORR_H
#define __CORR_H

#include <stdint.h>

void corr(int16_t* vectorX, int16_t* vectorY, int16_t* vectorCorr, uint32_t longitud);
void asm_corr(int16_t* vectorX, int16_t* vectorY, int16_t* vectorCorr, uint32_t longitud);

#endif /* __CORR_H */