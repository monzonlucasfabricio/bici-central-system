/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef TAREAS_H   /*! @cond    */
#define TAREAS_H   /*! @endcond */

/** @file leds.h
 ** @brief Declaraciones de la libreria para la gestion de los leds
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2020.12.30 | lucasmonzonl    | Version inicial del archivo para manejo de leds         |
 **
 ** @addtogroup leds
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "../../leds/include/leds.h"
#include "../../espnow/include/espnow_app.h"
#include "../../../main/main.h"

/* === Cabecera C++ ============================================================================ */
#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */
#define EVENT_TASK_PRIORITY 2
#define TASK_TIME_TO_DELETE 50
/* === Declaraciones de tipos de datos ========================================================= */

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */
void task_recv_msg(void *pvParameter);
void task_event_handler(void *pvParameter);
void task_sync_by_mac(void* pvParameter);
void command_task(void *pvParameter);
void moving_task(void *pvParameter);
void turn_task(void *pvParameter);
void emergency_task(void *pvParameter);


/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* TAREAS_H */