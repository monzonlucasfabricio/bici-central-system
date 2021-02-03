/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "main.h"



void app_main(void)
{
    // Initialize Non-Volatile-Storage
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    /* Mutex for connection structure created */
    conn_flag_semap = xSemaphoreCreateMutex();
    if (conn_flag_semap == NULL)
    {
        ESP_LOGE("MUTEX", "Mutex creation failed");
        while (1)
        {
        }
    }

    xSemaphoreFlags = xSemaphoreCreateMutex();
    if (xSemaphoreFlags == NULL)
    {
        ESP_LOGE("MUTEX", "Mutex creation failed");
        while (1)
        {
        }
    }

    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    /* Take mutex*/
    if (xSemaphoreTake(conn_flag_semap, portMAX_DELAY) == pdTRUE)
    {

        /* Check all for pairing device */
        ESP_ERROR_CHECK(f_second_device_init(&conn));
        ESP_LOGI("INFO", "Connection state: %s", state_to_string(conn.state));
        ESP_LOGI("INFO", "Second device MAC: %02X:%02X:%02X:%02X:%02X:%02X", conn.mac[0], conn.mac[1], conn.mac[2], conn.mac[3], conn.mac[4], conn.mac[5]);

        // WiFi initialization
        f_wifi_init();

        // Esp-now initialization
        if (f_esp_now_init() != ESP_OK)
        {
            ESP_LOGE("ESP_NOW", "System failed");
            while (true)
            {
            }
        }

        /* Give mutex */
        xSemaphoreGive(conn_flag_semap);
    }

    xTestHandle = NULL;
    xLeftHandle = NULL;
    xRightHandle = NULL;
    xMovingHandle = NULL;
    xEmergencyHandle = NULL;
    xTurnRightHandle = NULL;
    xTurnLeftHandle = NULL;
    xStoppedHandle = NULL;

    // Create recv task
    BaseType_t task_recv = xTaskCreate(task_recv_msg,
                                       "task_recv_msg",
                                       4 * configMINIMAL_STACK_SIZE,
                                       NULL,
                                       3,
                                       NULL);

    if (task_recv != pdPASS)
    {
        ESP_LOGE("TASK_RECV", "Error creating receive task");
        while (true)
        {
        }
    }

    // Create event handler
    BaseType_t task_evt_handler = xTaskCreate(task_event_handler,
                                              "task_recv_event",
                                              4 * configMINIMAL_STACK_SIZE,
                                              NULL,
                                              2,
                                              NULL);

    if (task_evt_handler != pdPASS)
    {
        ESP_LOGE("TASK_HANDLER", "Error creating event task");
        while (true)
        {
        }
    }
}
