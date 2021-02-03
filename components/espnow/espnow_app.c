
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
#include "include/espnow_app.h"
#include "../tasks/include/tasks.h"
#include "../nvs/include/nsv_second_dvce.h"
#include "../../main/main.h"


/* === Definicion y Macros ===================================================================== */

/* Name TAG for ESP_LOG library */
const char *TAG = "ESPNOW_MASTER_DEVICE";

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */

/* New mac address for the slave    -> unicast : less significant bit must be 0
                                    -> multicast : less significant bit must be 1*/
const uint8_t new_mac_address[ESP_NOW_ETH_ALEN] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x60};

/* === Declaraciones de funciones internas ===================================================== */

void OnReceiveData(const uint8_t *mac, const uint8_t *data, int len);

/* === Definiciones de funciones internas ====================================================== */

event_rcv_t internal_check_msb(uint8_t msb);
event_detail_t internal_check_lsb(uint8_t lsb,uint8_t evt);
esp_err_t internal_create_sync(uint8_t *mac);
esp_err_t internal_check_mac(uint8_t *mac);

/* === Definiciones de funciones externas ====================================================== */

/* WiFi should start before using ESPNOW */
void f_wifi_init(void)
{
    uint8_t *new_mac = pvPortMalloc(sizeof(uint8_t) * 6);

    /* Wifi should be disconnected to change the MAC base address */

    /* Set new base mac address and wifi station mode mac address */
    if (esp_base_mac_addr_set(new_mac_address) == ESP_OK)
    {
        ESP_LOGI(TAG, "Esp mac base address changed");
    }
    else
    {
        ESP_LOGE(TAG, "Error changing mac base address");
    }

    /* Read new mac address */
    if (esp_read_mac(new_mac, ESP_MAC_WIFI_STA) == ESP_OK)
    {
        ESP_LOGI(TAG, "MAC ADDRESS IS: %02x:%02x:%02x:%02x:%02x:%02x\n", new_mac[0], new_mac[1], new_mac[2], new_mac[3], new_mac[4], new_mac[5]);
    }

    /* Free memory allocated */
    vPortFree(new_mac);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());

#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK(esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
#endif
}

esp_err_t f_esp_now_init(void)
{

    if (esp_now_init() == ESP_OK)
    {
        ESP_LOGI(TAG, "Esp-now successfully initialized");
    }
    else
    {
        ESP_LOGE(TAG, "Error initializing esp-now");
        while (true)
        {
        }
    }

    /* Queue create */
    rcv_queue = xQueueCreate(EVENT_QUEUE_SIZE, sizeof(msg_t));
    if (rcv_queue == NULL)
    {
        ESP_LOGE(TAG, "Queue creation failed");
        return ESP_FAIL;
    }

    evt_queue = xQueueCreate(EVENT_QUEUE_SIZE, sizeof(event_t));
    if (evt_queue == NULL)
    {
        ESP_LOGE(TAG, "Queue creation failed");
        return ESP_FAIL;
    }

    /* Register callback function */
    esp_now_register_recv_cb(OnReceiveData);

    /* Sync flag start false */
    conn.flag_sync = false;

    return ESP_OK;
}

void OnReceiveData(const uint8_t *mac, const uint8_t *data, int len)
{
    /* Allocate memory for msg_t struct */
    msg_t *msg_recv = pvPortMalloc(sizeof(msg_t));
    if (msg_recv == NULL)
    {
        ESP_LOGE(TAG, "Can't allocate memory for msg");
    }

    /* Blink led for debug */
    gpio_set_level(BLINK_GPIO, HIGH);

    for (uint8_t i = 0; i < ESP_NOW_ETH_ALEN; i++)
    {
        msg_recv->mac_recv[i] = mac[i];
    }

    msg_recv->len = len;
    msg_recv->msb_byte = *(data);
    msg_recv->lsb_byte = *(data + 1);

    /* Send msg to the queue */
    if (xQueueSend(rcv_queue, msg_recv, portMAX_DELAY) != pdTRUE)
    {
        ESP_LOGE(TAG, "Error sending msg to the queue");
        vPortFree(msg_recv);
    }

    vPortFree(msg_recv);
    gpio_set_level(BLINK_GPIO, LOW);
}


esp_err_t f_parser_data(uint8_t msb,uint8_t lsb,uint8_t *mac){

    event_t *evt = pvPortMalloc(sizeof(event_t));
    if(evt == NULL){
        ESP_LOGE(TAG, "Can't allocate memory for event");
        return ESP_FAIL;
    }

    /* Check the event received */
    evt->evt_rcv = internal_check_msb(msb);
    if (evt->evt_rcv == FAIL_R){
        ESP_LOGI(TAG, "Event received FAILED");
    }

    /* Check the detail of the event received */
    evt->evt_det = internal_check_lsb(lsb,evt->evt_rcv);
    if (evt->evt_det == FAIL_D){
        ESP_LOGI(TAG, "Event detail FAILED");
    }

    if ( evt->evt_rcv != FAIL_R && evt->evt_det != FAIL_D){
        /* Take mutex so can check synchronization flag*/
        if (xSemaphoreTake( conn_flag_semap, portMAX_DELAY ) == pdTRUE){
            if (evt->evt_rcv == COMMAND && evt->evt_det == SYNC && conn.flag_sync == true){
                ESP_LOGI(TAG, "Try to syncronize device");

                /* Create a syncronization task */
                internal_create_sync(mac);
            }

            else if (internal_check_mac(mac) == ESP_OK){
                /* Send msg to the queue */
                if (xQueueSend(evt_queue, evt, portMAX_DELAY) != pdTRUE)
                {
                    ESP_LOGE(TAG, "Error sending msg to the queue");
                    vPortFree(evt);
                }
            }

            /* Give mutex*/
            xSemaphoreGive(conn_flag_semap);
        }

    }

    vPortFree(evt);

    return ESP_OK;
}

event_rcv_t internal_check_msb(uint8_t msb){

    switch(msb){
        case EVENT_MOV:{return MOV;}break;
        case EVENT_TURN:{return TURN;}break;
        case EVENT_EMERGENCY:{return EMERGENCY_R;}break;
        case EVENT_COMMAND:{return COMMAND;}break;
        default:{return FAIL_R;}break;
    }
}

event_detail_t internal_check_lsb(uint8_t lsb,uint8_t evt){

    if ( evt == COMMAND){
        switch(lsb){
            case DESC_LEFT_OFF:{return LEFT_OFF;}break;
            case DESC_LEFT_ON:{return LEFT_ON;}break;
            case DESC_RIGHT_OFF:{return RIGHT_OFF;}break;
            case DESC_RIGHT_ON:{return RIGHT_ON;}break;
            case DESC_TEST_OFF:{return TEST_OFF;}break;
            case DESC_TEST_ON:{return TEST_ON;}break;
            case DESC_SYNC:{return SYNC;}break;
        }
    }

    if (evt == EMERGENCY_R){
        switch(lsb){
            case DESC_EMERGENCY:{return EMERGENCY_D;}break;
        }
    }

    if (evt == MOV){
        switch(lsb){
            case DESC_MOVING:{return MOVING;}break;
            case DESC_STOPPED:{return STOPPED;}break;
    }
    }

    if(evt == TURN){
        switch(lsb){
            case DESC_TURN_RIGHT:{return TURN_RIGH;}break;
            case DESC_TURN_LEFT:{return TURN_LEFT;}break;
        }
    }

    return FAIL_D;
}

esp_err_t internal_create_sync(uint8_t *mac){

    uint8_t *param = pvPortMalloc(sizeof(uint8_t)*ESP_NOW_ETH_ALEN);
    if (param == NULL){
        vPortFree(param);
        return ESP_FAIL;
    }

    memcpy(param,mac,sizeof(uint8_t)*ESP_NOW_ETH_ALEN);

    BaseType_t ret = xTaskCreate(task_sync_by_mac, "Task sync",4*configMINIMAL_STACK_SIZE,(void*)param,2,NULL);
    if (ret != pdPASS){
        ESP_LOGE(TAG, "Error creating sync task");
        while(true){}
    }

    return ESP_OK;
}


esp_err_t f_sync_new_device(uint8_t *mac){

    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
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
        /* Write new MAC address to the device */
        required_size = sizeof(uint8_t) * ESP_NOW_ETH_ALEN;
        memcpy(sec_device,mac,required_size);
        err = nvs_set_blob(my_handle, "sec_device", sec_device, required_size);

        /* Synchronized mode ON*/
        conn.state = SYNCHRONIZED;
        memcpy(&conn.mac,mac,sizeof(uint8_t)*ESP_NOW_ETH_ALEN);

        vPortFree(sec_device);

        if (err != ESP_OK)
            return err;

        // Commit
        err = nvs_commit(my_handle);
        if (err != ESP_OK)
            return err;

        // Close
        nvs_close(my_handle);
    }
    else{
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t internal_check_mac(uint8_t *mac){
    uint8_t ret = compareArray(conn.mac,mac,sizeof(ESP_NOW_ETH_ALEN));
    if(ret == 0){
        ESP_LOGI("MAC CHECK", "Mac address matched");
        return ESP_OK;
    }
    else{
        ESP_LOGI("MAC CHECK", "Mac address not matched");
        return ESP_FAIL;
    }
}
