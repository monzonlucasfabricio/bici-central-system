/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef EVENTS_DISTPACH   /*! @cond    */
#define EVENTS_DISTPACH   /*! @endcond */

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
#include "../../espnow/include/espnow_app.h"
#include "../../tasks/include/tasks.h"

/* === Cabecera C++ ============================================================================ */
#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */
#define QUEUE_SIZE_GENERIC 10
#define QUEUE_TIME_WAIT 10

uint8_t Command_storage_area[ QUEUE_SIZE_GENERIC * sizeof(uint8_t)];
uint8_t Emergency_storage_area[ QUEUE_SIZE_GENERIC * sizeof(uint8_t)];
uint8_t Turn_storage_area[ QUEUE_SIZE_GENERIC * sizeof(uint8_t)];
uint8_t Moving_storage_area[ QUEUE_SIZE_GENERIC * sizeof(uint8_t)];

xQueueHandle command_queue;
xQueueHandle emergency_queue;
xQueueHandle turn_queue;
xQueueHandle moving_queue;


/* === Declaraciones de tipos de datos ========================================================= */

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

esp_err_t f_task_dispatcher(event_t *evt);
esp_err_t f_queues_for_events(void);
esp_err_t f_event_task_creates(void);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* EVENTS_DISTPACH */