/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file events_dispatch.c
 ** @brief Leds library. Just to show how to organize the files.
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.12 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup events
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "include/events_dispatch.h"

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de funciones internas ====================================================== */

/* === Definiciones de funciones externas ====================================================== */

esp_err_t f_task_dispatcher(event_t *evt)
{
    char TAG[] = "DISPATCHING";
    switch (evt->evt_rcv)
    {
    case COMMAND:
    {
        if (xQueueSend(command_queue, &evt->evt_det, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGE(TAG, "Error sending command to the queue");
        }
        else{
            ESP_LOGI(TAG, "Command dispatched");
            return ESP_OK;
        }
    }
    break;
    case EMERGENCY_R:
    {
        if (xQueueSend(emergency_queue, &evt->evt_det, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGE(TAG, "Error sending emergency to the queue");
        }
        else{
            ESP_LOGI(TAG, "Emergency dispatched");
            return ESP_OK;
        }
    }
    break;
    case TURN:
    {
        if (xQueueSend(turn_queue, &evt->evt_det, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGE(TAG, "Error sending turn to the queue");
        }
        else{
            ESP_LOGI(TAG, "Turn dispatched");
            return ESP_OK;
        }
    }
    break;
    case MOV:
    {
        if (xQueueSend(moving_queue, &evt->evt_det, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGE(TAG, "Error sending moving to the queue");
        }
        else{
            ESP_LOGI(TAG, "Moving dispatched");
            return ESP_OK;
        }
    }
    break;
    case FAIL_R:
    {
        ESP_LOGI(TAG,"FAIL_R");
        return ESP_FAIL;
    }
    break;
    }
    return ESP_FAIL;
}

esp_err_t f_queues_for_events(void)
{

    command_queue = xQueueCreate(QUEUE_SIZE_GENERIC, sizeof(uint8_t));
    emergency_queue = xQueueCreate(QUEUE_SIZE_GENERIC, sizeof(uint8_t));
    moving_queue = xQueueCreate(QUEUE_SIZE_GENERIC, sizeof(uint8_t));
    turn_queue = xQueueCreate(QUEUE_SIZE_GENERIC, sizeof(uint8_t));

    if (command_queue == NULL || command_queue == NULL || command_queue == NULL || command_queue == NULL)
    {
        ESP_LOGE("DISPATCHER", "Queue creation failed");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t f_event_task_creates(void)
{
    char TAG[] = "EVENT CREATION";
    BaseType_t ret1 = xTaskCreate(command_task, "Command", 4 * configMINIMAL_STACK_SIZE,NULL, 3, NULL);
    BaseType_t ret2 = xTaskCreate(emergency_task, "Emergency", 4 * configMINIMAL_STACK_SIZE,NULL, 3 , NULL);
    BaseType_t ret3 = xTaskCreate(turn_task, "Turn", 4 * configMINIMAL_STACK_SIZE,NULL, 3, NULL);
    BaseType_t ret4 = xTaskCreate(moving_task, "Moving", 4 * configMINIMAL_STACK_SIZE,NULL, 3, NULL);

    if (ret1 != pdPASS || ret2 != pdPASS || ret3 != pdPASS || ret4 != pdPASS)
    {
        ESP_LOGE(TAG, "Error creating events tasks");
        while (true)
        {
        }
    }
    
    return ESP_OK;
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */