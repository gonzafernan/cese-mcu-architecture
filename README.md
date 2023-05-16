---
title: Arquitectura de Microprocesadores
author: Gonzalo G. Fernandez
---
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

Comparación de arquitecturas:

|                         | Cortex-M0 y M0+ | Cortex-M3 | Cortex-M4 | Cortex-M7 |
|-------------------------|-----------------|-----------|-----------|-----------|
| Arquitectura ARM        | ARMv6-M         | ARMv7-M   | ARMv7E-M  | ARMv7E-M  |
| Arquitectura de memoria | Von-Neumann     | Harvard   | Harvard   | Harvard   |
| CPU cache               | No              | No        | Posible   | Opcional  |
| Memotia altamente acoplada (TCM) | No | No | No | Opcional |
| [Unidad de Protección de Memoria (MPU)](README.md#unidad-de-protección-de-memoria-mpu) | No (Opcional en M0+) | Opcional | Opcional | Opcional |
| Bit-banding             | Opcional        | Si        | Si        | Si        |
| [SysTick Timer](README.md#systick-timer) | Opcional | Si | Si     | Si        |

Comparación de extensiones:

|                         | Cortex-M0 y M0+ | Cortex-M3 | Cortex-M4 | Cortex-M7 |
|-------------------------|-----------------|-----------|-----------|-----------|
| Unidad de punto flotante| No              | No        | Opcional  | Opcional  |
| Extensiones para DSP    | No              | No        | Si        | Si        |
| Aritmética saturada     | No              | Si        | Si        | Si        |
| División por hardware   | No              | Si        | Si        | Si        |
| Multiplicación por hardware | Resultado de 32-bit | Resultado de 32 o 64-bit | Resultado de 32 o 64-bit | Resultado de 32 o 64-bit |

A partir del procesador Cortex-M3 se soporta todo Thumb-2.

## Tecnología Thumb-2
Todos los procesadores Cortex-M poseen tecnología Thumb-2 y diferentes subsets del ISA Thumb. Previo a Thumb-2, la ISA Thumb era solo para set de instrucciones de 16 bits. Thumb-2 extendió la ISA a un set altamente potente y eficiente que otorga beneficios considerables en términos de facilidad de uso, tamaño de código y rendimiento.

Al permitir instrucciones tanto de 16 como 32 bits, Thumb-2 elimina la necesidad de conmutar el procesador entre estado Thumb (instrucciones de 16 bits) y estado ARM (instrucciones de 32 bits). Esto le permite a los procesadores Cortex-M mezclar instrucciones de 16 y 32 bits sin cambiar de estado, lo que conlleva una gran densidad de código y alto rendimiento, sin añadir complejidad.

## Arquitecture load-store
La arquitectura load-store es una arquitectura cuyo set de instrucciones se divide en dos categorías: De acceso a memoria, asociada a la carga y almacenamiento entre memoria y registros, y operaciones de ALU (unidad aritmética lógica) que solo ocurren entre registros. Si hay información en memoria a ser procesada, primero debe ser cargada en registros, procesada en el procesador y escrita nuevamente en memoria de ser necesario. 

Por ejemplo, en una arquitectura load-store, ambos operandos de una operación ADD deben ser registros. Esto difiere de una arquitectura registro-memoria (por ejemplo CISC) donde alguno de los operandos para la operación ADD  puede encontrarse en memoria y el otro en un registro.

La familia Cortex de ARM  (y en general cualquiera de ARM) es una arquitectura load-store.

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

## Shadowed stack pointer: MSP y PSP
Hay dos stack pointers en los procesadores Cortex-M:

- El MSP (Main Stack Pointer) es el stack pointer por defecto. Es utilizado en modo Thread cuando el bit 1 de CONTROL (SPSEL) es 0, y es utilizado siempre en mode Handle.
- El PSP (Processor Stack Pointer) es el stack pointer utilizado en modo Thread cuando el bot 1 de CONTROL (SPSEL) es 1.

Operaciones de stack como las instrucciones PUSH y POP, y la mayoría de las instrucciones que utilizan el registro SP (R13) utilizan el stack pointer que se encuentre seleccionado en ese momento. 
También se puede acceder al MSP y PSP directamente utilizando las instrucciones MRS y MSR. 

En las aplicaciones simples, sin un RTOS, se puede utilizar solo el MSP e ignorar el PSP.
En sistemas con un RTOS, la gestión de interrupciones utiliza MSP, mientras que las tareas de la aplicación utilizan el PSP. Cada tarea de la aplicación posee su propio espacio de stack y en el cambio de contexto el RTOS actualiza el PSP al espacio correspondiente.

## Stack
El stack (o pila) es un mecanismo para uso de memoria que permite usar una porción de memoeria como un buffer LIFO (Last-In-First-Out).
Los princepales instrucciones a ejecutar sobre el stack son PUSH, para guardar un dato, y POP, para obtener un dato.
Cada vez que se ejecuta una instrucción PUSH o POP, se opera sobre el stack que este seleccionado en ese momento.

El stack puede ser utilizado para:

- Almacenamiento temporal de información cuando una función ejecutada necesita utilizar un registro.
- Pasar información a funciones o subrutinas (argumentos).
- Para almacenar variables locales.
- Para mantener el estado del procesador y el valor de los registros en caso de una excepción/interrupción.

Los procesadores Cortex-M utilizan un modelo de memoria de stack denominado "full-descending stack". En la siguiente figura se observa una representación de las operaciones de PUSH y POP:

![Stack PUSH y POP](imgs/stack_push_pop.png)

### Uso de PUSH y POP en funciones
Uno de los usos más comunes de las instrucciones PUSH y POP es almacenar el contenido de los registros cuando es ejecuta el llamado a una función o subrutina.

En la siguiente figura se puede observar el llamado a la función *function1*:

![Uso de PUSH y POP en funciones](imgs/stack_function_call.png)

Como *function1* nesecita utilizar y modificar los registros R4, R5 y R6, y esos registros contienen información que debe ser utilizada por el programa principal una vez que finalice la función, se pueden almacenar en el stack utilizando PUSH y recuperarlos con POP al final de la función.

**NOTA: Prestar especial atención al orden de los diferentes PUSH y POP.**

Se realizar PUSH y POP de múltiples registros de la siguiente forma:

```asm
function1
    PUSH {R4-R6}    ; Store R4, R5, R6 to stack
    ...
    POP {R4-R6}     ; Restore R4, R5, R6
    BX LR           ; Return
```

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