
/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file espnow_app.c
 ** @brief espnow functions  
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.13 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup espnow
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "include/nsv_second_dvce.h"

#define STORAGE_NAMESPACE "storage"

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */
uint8_t mac_default[ESP_NOW_ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de funciones internas ====================================================== */

/* === Definiciones de funciones externas ====================================================== */

esp_err_t f_second_device_init(connection_state_t *conn)
{

    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    // GET BLOB SIZE
    size_t required_size = 0; // value will default to 0, if not set yet in NVS
    err = nvs_get_blob(my_handle, "sec_device", NULL, &required_size);
    switch (err)
    {
    case ESP_OK:
    {
        ESP_LOGI("NVS_CHECK", "Sec_device key exists");
    }
    break;
    case ESP_ERR_NVS_NOT_FOUND:
    {
        ESP_LOGI("NVS_CHECK", "Key doesn't exist");
    }
    break;
    default:
    {
        return err;
    }
    break;
    }

    
    /* If required size is greater than 0 means that it was writen */
    uint8_t *sec_device = malloc(sizeof(uint8_t) * ESP_NOW_ETH_ALEN);
    if (required_size > 0)
    {
        err = nvs_get_blob(my_handle, "sec_device", sec_device, &required_size);
        if (err != ESP_OK)
        {
            free(sec_device);
            return err;
        }
        else
        {
            memcpy(&conn->mac,sec_device,sizeof(uint8_t)*ESP_NOW_ETH_ALEN);
            if (compareArray(mac_default, sec_device, ESP_NOW_ETH_ALEN - 1) == 0)
            {
                /* If arrays are equal means that have never been synced, so mac address is FF:FF:FF:FF:FF:FF */
                conn->state = OUTOFSYNC;
            }
            else
            {
                /* If arrays aren't equal means that it was syncronized but doesn't mean is the actual second device */
                conn->state = SYNCHRONIZED;
            }
        }
    }
    else
    {
        // Write value including previously saved blob if available
        required_size = sizeof(uint8_t) * ESP_NOW_ETH_ALEN;
        memcpy(sec_device,mac_default,required_size);
        err = nvs_set_blob(my_handle, "sec_device", sec_device, required_size);
        free(sec_device);

        if (err != ESP_OK)
            return err;

        // Commit
        err = nvs_commit(my_handle);
        if (err != ESP_OK)
            return err;

        // Close
        nvs_close(my_handle);
    }

    return ESP_OK;
}

uint8_t compareArray(uint8_t a[], uint8_t b[], int sizeinside)
{
    int i;
    for (i = 0; i < sizeinside; i++)
    {
        if (a[i] != b[i])
            return 1;
    }
    return 0;
}

char* state_to_string(state_t state){
    if (state == OUTOFSYNC){
        return "Out_of_sync";
    }
    else if (state == SYNCHRONIZED){
        return "Synchronized";
    }
    return "Fail\n";
}