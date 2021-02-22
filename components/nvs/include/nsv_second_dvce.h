/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef NVS_SECOND_DVCE_H   /*! @cond    */
#define NVS_SECOND_DVCE_H   /*! @endcond */

/** @file espnow_app.h
 ** @brief Library declaration for espnow functions and defines
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.13 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup ESP_NOW
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */


#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_crc.h"
#include "driver/gpio.h"

/**
 * @brief Enum for second device states
 * 
 */
typedef enum state_t{
    SYNCHRONIZED,
    OUTOFSYNC
}state_t;

/**
 * @brief 
 * 
 */
typedef struct connection_state_t{
    state_t state;
    uint8_t mac[ESP_NOW_ETH_ALEN];
    bool flag_sync;
}connection_state_t;

esp_err_t f_second_device_init(connection_state_t *conn);
uint8_t compareArray(uint8_t a[], uint8_t b[], int sizeinside);
char* state_to_string(state_t state);



#endif //NVS_SECOND_DVCE_H