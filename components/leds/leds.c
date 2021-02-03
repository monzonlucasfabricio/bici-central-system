/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file leds.c
 ** @brief Leds library. Just to show how to organize the files.
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.12 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup leds
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "include/leds.h"

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de funciones internas ====================================================== */

/* === Definiciones de funciones externas ====================================================== */

esp_err_t createLed(uint8_t pinled, modes_s mode, ledsparams_t *led)
{
    ledsparams_t newled = {
        .pinLed = pinled,
        .mode = mode,
    };

    memcpy(led, &newled, sizeof(ledsparams_t));

    return ESP_OK;
}

/* ----------- LEDS TASK SECTION -------------------------------------*/
/* This will change location to tasks.c */

void f_test_lights(void *pvParameter)
{
    //printf("Tarea TEST creada\n");
    while (true)
    {
        printf("------------> TEST ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
void f_left_lights(void *pvParameter)
{
    //printf("Tarea LEFT creada\n");
    while (true)
    {
        printf("------------> LEDS LEFT ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
void f_right_lights(void *pvParameter)
{
    //printf("Tarea RIGHT creada\n");
    while (true)
    {
        printf("------------> LEDS RIGHT ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void f_moving(void *pvParameter)
{
    //printf("Tarea Moving creada\n");
    while (true)
    {
        printf("------------> Moving ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}


void f_turn_right(void *pvParameter)
{
    //printf("Tarea TURN RIGHT creada\n");
    while (true)
    {
        printf("------------> TURN RIGHT ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void f_turn_left(void *pvParameter)
{
    //printf("Tarea TURN LEFT creada\n");
    while (true)
    {
        printf("------------> TURN LEFT ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void f_emergency(void *pvParameter)
{
    //printf("Tarea Emergency creada\n");
    while (true)
    {
        printf("------------> EMERGENCY ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void f_stopped(void *pvParameter)
{
    //printf("Tarea Emergency creada\n");
    while (true)
    {
        printf("------------> STOPPED ON\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}




/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */