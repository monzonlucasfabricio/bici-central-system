/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef MAIN_H   /*! @cond    */
#define MAIN_H   /*! @endcond */

/** @file espnow_app.h
 ** @brief Library declaration for espnow functions and defines
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.13 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup MAIN
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"

/* Include every component header file like the next lines of code. With all the file path. */
#include "../components/espnow/include/espnow_app.h"
#include "../components/tasks/include/tasks.h"
#include "../components/nvs/include/nsv_second_dvce.h"
#include "../components/events/include/events_dispatch.h"

/* Connection structure global */
connection_state_t conn;

/* This mutex is used every time I check Sync_flag in conn structure*/
SemaphoreHandle_t conn_flag_semap;

/* Semaphores for commands */
SemaphoreHandle_t xSemaphoreFlags;

/* Global variables to check states */
TaskHandle_t xTestHandle;
TaskHandle_t xLeftHandle;
TaskHandle_t xRightHandle;
TaskHandle_t xMovingHandle;
TaskHandle_t xEmergencyHandle;
TaskHandle_t xTurnRightHandle;
TaskHandle_t xTurnLeftHandle;
TaskHandle_t xStoppedHandle;

#endif //MAIN_H