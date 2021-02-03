/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file test_leds.c
 ** @brief Unit test for leds component file  
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.03 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup leds
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include <string.h>
#include "unity.h"
#include "../include/leds.h"

/* === Definicion y Macros ===================================================================== */
#define LEDPIN 1

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */
ledsparams_t led;
/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de funciones internas ====================================================== */

/* === Definiciones de funciones externas ====================================================== */

void setUp(void) {
    esp_err_t ret = createLed(LEDPIN,OUTPUT,&led);
}

void tearDown(void) {

}


/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */
