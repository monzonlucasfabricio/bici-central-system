/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef ESPNOW_APP_H   /*! @cond    */
#define ESPNOW_APP_H   /*! @endcond */

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


#define CONFIG_ESPNOW_WIFI_MODE_STATION 1

#define BLINK_GPIO 2
#define HIGH 1
#define LOW 0
#define EVENT_QUEUE_SIZE 10

/* Events define in HEX */
#define EVENT_MOV           0x00
#define EVENT_TURN          0xF0
#define EVENT_EMERGENCY     0xFF
#define EVENT_COMMAND       0x0F

/* Events details in HEX */
#define DESC_MOVING         0xF0
#define DESC_STOPPED        0x0F
#define DESC_TURN_RIGHT     0x0F
#define DESC_TURN_LEFT      0xF0
#define DESC_EMERGENCY      0x00
#define DESC_LEFT_OFF       0xC0
#define DESC_LEFT_ON        0xCF
#define DESC_RIGHT_OFF      0x30
#define DESC_RIGHT_ON       0x3F
#define DESC_SYNC           0x66
#define DESC_TEST_ON        0x9F
#define DESC_TEST_OFF       0x90

/* Queue definition */
xQueueHandle rcv_queue;
xQueueHandle evt_queue;

/* ESPNOW can work in both: station and softap mode. It is configured in menuconfig.*/

#if CONFIG_ESPNOW_WIFI_MODE_STATION

/*STATION MODE (aka STA mode or WiFi client mode). ESP32 connects to an access point.*/
#define ESPNOW_WIFI_MODE    WIFI_MODE_STA
#define ESPNOW_WIFI_IF      ESP_IF_WIFI_STA
#else

/*ACCESS POINT mode (aka Soft-AP mode or Access Point mode). Stations connect to the ESP32.*/
#define ESPNOW_WIFI_MODE    WIFI_MODE_AP
#define ESPNOW_WIFI_IF      ESP_IF_WIFI_AP
#endif

/**
 * @brief Event type enum for every event received that can be handled on this device
 * 
 */
typedef enum event_rcv_t{
    COMMAND,
    EMERGENCY_R,
    TURN,
    MOV,
    FAIL_R,
}event_rcv_t;

/**
 * @brief Event type enum for every subevent received that can be handled on this device
 * 
 */
typedef enum event_detail_t{
    LEFT_OFF,
    LEFT_ON,
    RIGHT_OFF,
    RIGHT_ON,
    SYNC,
    TEST_OFF,
    TEST_ON,
    EMERGENCY_D,
    MOVING,
    STOPPED,
    TURN_RIGH,
    TURN_LEFT,
    FAIL_D,
}event_detail_t;

/**
 * @brief General event structure that holds event and subevent
 * 
 */
typedef struct event_t{
    event_detail_t evt_det;
    event_rcv_t evt_rcv;
    uint8_t mac[ESP_NOW_ETH_ALEN];
}event_t;

/**
 * @brief Message received structure, holds msb, lsb and mac address
 * 
 */
typedef struct msg_t{
    uint8_t mac_recv[ESP_NOW_ETH_ALEN];
    uint8_t lsb_byte;
    uint8_t msb_byte;
    int len;
}msg_t;



/**
 * @brief Wifi initialization funtion, it changes the base MAC address
 * 
 */
void f_wifi_init(void);

/**
 * @brief Esp-now initializaction function
 * 
 * @return esp_err_t    -> ESP_OK if everything went OK
 *                      -> ESP_FAIL if something went wrong
 */
esp_err_t f_esp_now_init(void);

/**
 * @brief Function that parse data from the message received on esp-now.
 * 
 * @param msb -> Most significant byte
 * @param lsb -> Less significant byte
 * @param mac -> Mac address received from the second-device
 * @return esp_err_t    -> ESP_OK if the parser was ok and memory was available
 *                      -> ESP_FAIL if memory wasn't available 
 */
esp_err_t f_parser_data(uint8_t msb, uint8_t lsb,uint8_t *mac);


/**
 * @brief Function that access to the Non Volatile Storage and changes the old MAC address for the new device.
 * (TODO:This function should be in NVS component)
 * @param mac -> New MAC address from the second-device
 * @return esp_err_t 
 */
esp_err_t f_sync_new_device(uint8_t *mac);



#endif //ESPNOW_APP_H