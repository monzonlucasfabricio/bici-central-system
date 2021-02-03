/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef LEDS_H   /*! @cond    */
#define LEDS_H   /*! @endcond */

/** @file leds.h
 ** @brief Library declaration for the leds test library.
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.11 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup leds
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* === Cabecera C++ ============================================================================ */
#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

typedef enum{
    INPUT,
    OUTPUT,
}modes_s;

typedef struct{
    uint8_t pinLed;
    modes_s mode;
}ledsparams_t;

esp_err_t f_create_led(uint8_t pinled, modes_s mode,ledsparams_t *led);

void f_test_lights(void *pvParameter);
void f_left_lights(void *pvParameter);
void f_right_lights(void *pvParameter);
void f_moving(void *pvParameter);
void f_turn_right(void *pvParameter);
void f_turn_left(void *pvParameter);
void f_emergency(void *pvParameter);
void f_stopped(void *pvParameter);


/* === Declaraciones de tipos de datos ========================================================= */

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* LEDS_H */