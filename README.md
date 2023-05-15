Carrera de Especialización en Sistemas Embebidos - Universidad de Buenos Aires
---
# Arquitectura de Microprocesadores

## Familias de microprocesadores/microcontroladores de ARM
### ARM Cortex A (Application profile)
Proveen una variedad de soluciones para dispositivos que hacen uso intensivo de sistemas operativos de propósito general.
Están optimizados para correr múltiples aplicaciones a la vez. No están orientado a cumplir requisitos de tiempo real.
- Motor de procesamiento de media NEON
- Trustzone para extensiones de seguridad
- Soporte para punto flotante de precisión single y double
- Soporte para varios sets de instrucciones

### ARM Cortex R (Real-time profile)
Los procesadores Cortex-R apuntan a aplicaciones de tiempo real de alto rendimiento, como controladores de discos duros, equipos de redes, aplicaciones de la industria automotriz como airbags, sistemas de freno, etc. Es similar a un microcontrolador de alto rendimiento pero apunta a sistemas más grandes.

Utilizarán un sistema operativo de tiempo real.

### ARM Cortex M (Microcontroller profile)
Es un Cortex-R pero más reducido, simple y de menor costo. Apuntan al mercado de microcontroladores.
Se los puede encontrar implementado como soft-core en FPGAs, pero es más común encontrarlos en microcontroladores con memorias integradas, relojes y periféricos.

Algunos se encuentran optimizados para un consumo eficiente de energía, otros para alto rendimiento y otros para mercados específicos como puede ser el de dispositivos inteligentes.

Los Cortex-M3 y Cortex-M4 son núcleos similares, la principal diferencia es la capacidad de DSP de los Cortex-M4. Ambos utilizan el set de instrucciones Thumb-2.

Por debajo de los Cortex-M3 y Cortex-M4 se ubican los Cortex-M0 y Cortex-M0+. Apuntan a una implementación mínima para bajo consumo y bajo costo. Utilizan un subset de Thumb-2.

## Diferencia entre familias de procesadores Cortex

|                         | Cortex-M0 y M0+ | Cortex-M3 | Cortex-M4 | Cortex-M7 |
|-------------------------|-----------------|-----------|-----------|-----------|
| Arquitectura ARM        | ARMv6-M         | ARMv7-M   | ARMv7E-M  | ARMv7E-M  |
| Arquitectura de memoria | Von-Neumann     | Harvard   | Harvard   | Harvard   |
| CPU cache               | No              | No        | Posible   | Opcional  |
| Memotia altamente acoplada (TCM) | No | No | No | Opcional |
| [Unidad de Protección de Memoria (MPU)](README.md#unidad-de-protección-de-memoria-mpu) | No (Opcional en M0+) | Opcional | Opcional | Opcional |
| Bit-banding             | Opcional        | Si        | Si        | Si        |
| [SysTick Timer](README.md#systick-timer) | Opcional | Si | Si     | Si        |


## Mapa de memoria de ARM Cortex-M
Con 32 bits para direccionar, los procesadores ARM pueden acceder hasta 4GB de espacio de memoria.
Los 4GB del espacio de dirección de los Cortex-M se particiona en un determinado número de regiones de memoria. La partición se basa en el tipo de uso, de tal forma que las diferentes áreas se diseñan para utilizarse principalmente en:

- Código del progrma
- Acceso a información
- Periféricos
- Control interno del procesador y componentes para debug

La arquitectura permite gran flexibilidad para permitir que las regiones de memoria sean utilizadas con otros propósitos.

En la práctica muchos microcontroladores solo utilizan una porcion de cada región para flash, SRAM y periféricos.

El arreglo del mapa de memoria es consistente entre todos los procesadores Cortex-M. Esto hace que sea más facil portar software de un Cortex-M a otro y permite una mejor reusabilidad de código.

A continuación se observa el mapa de memoria de los procesadores Cortex-M:

![Memory Map](imgs/mamory_map.png)


## Arquitecture load-store
La arquitectura load-store es una arquitectura cuyo set de instrucciones se divide en dos categorías: De acceso a memoria, asociada a la carga y almacenamiento entre memoria y registros, y operaciones de ALU (unidad aritmética lógica) que solo ocurren entre registros. Si hay información en memoria a aser procesada, primero debe ser cargada en registros, procesada en el procesador y escrita nuevamente en memoria de ser necesario. 

Por ejemplo, en una arquitectura load-store, ambos operandos de una operación ADD deben ser registros. Esto difiere de una arquitectura registro-memoria (por ejemplo CISC) donde alguno de los operandos para la operación ADD  puede encontrarse en memoria y el otro en un registro.

La familia Cortex de ARM  (y en general cualquiera de ARM) es una arquitectura load-store.

## SysTick timer
Los procesadores Cortex-M tienen integrado un timer pequeño denominado SysTick (System Tick). Es un timer de 24 bits de decremento simple y puede ejecutarse con la frecuencia de clock del procesador o de una referencia de clock externa.

En los RTOS es necesario una interrupción periódica que permite invocar al kernel del sistema operativo para, por ejemplo, la gestión de tareas y el cambio de contexto. El diseño del procesador también es tal que no permite que tareas en modo no privilegiado puedan deshabilitar el timer.

La razón por la que el timer se encuentra integrado dentro del procesador es ayudar a la portabilidad del software. Dado que todos los procesadores Cortex-M tienen el mismo timer SysTick, un sistema operativo desarrollado para una Cortex-M3/M4 determinado puede reutilizarse en otro microcontrolador Cortex-M3/M4.

Si la aplicación no utiliza un sistema operativo, el SysTick puede utilizarse como un timer convencional que permita generar interrupciones de forma periódica, generación de delays o medición de lapsos de tiempo.

## Unidad de protección de memoria (MPU)
La MPU es un dispositivo programable que puede utilizarse para definir permisos de acceso a memoria y atributos de memoria en diferentes regiones de la misma.

La MPU es opcional en procesadores Cortex-M3 y Cortex-M4, y aquellos que lo poseen soportan haste 8 regiones de memoria programables, cada uno con su propia dirección de inicio, tamaño y configuración.

En sistemas embebidos que requieren robustez y gran fiabilidad, la MPU puede utilizarse para:
- Prevenir que tareas de aplicación de corrompan la pila o información en memoria que es utilizada por otras tareas o el kernel del sistema operativo.
- Prevenir que tareas sin privilegios accedan a ciertos periféricos que pueden ser críticos en la fiabilidad y seguridad del sistema
- Definir espacio de SRAM o RAM como no ejecutable para prevenir ataques por inyección.

Si un acceso a memoria viola los permisos de acceso definidos por la MPU, la transferencia es bloqueada y se levanta una excepción. El gestor de la excepción luego decidirá si el sistema debe resetearse o únicamente se termina la tarea que produjo la excepción en un enterno con sistema operativo.

La MPU necesita ser programada y habilitada previo a utilizarse. Las regiones de la MPU puede sobreponerse. Si un sector memoria cae entre dos regiones programados por la MPU, los atributos de acceso y permisos se definen basándose en la región con valor numérico más alto. Por ejemplo, si una dirección de transferencia se ubica dentro del rango definido por la región 1 y la región 4, se utilizará la configuración de la región 4.