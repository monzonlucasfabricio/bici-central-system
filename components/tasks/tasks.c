#include "include/tasks.h"
#include "../../components/events/include/events_dispatch.h"

void f_delete_tasks(void);

void task_recv_msg(void *pvParameter)
{
    char TAG[] = "TASK_RECV";
    ESP_LOGI(TAG, "RECEIVED TASK INITIALIZED");
    msg_t msg;
    msg_t *msg_recv = &msg;

    while (true)
    {
        if (rcv_queue != NULL)
        {
            if (xQueueReceive(rcv_queue, msg_recv, portMAX_DELAY) == pdTRUE)
            {
                //printf("PUNTERO 2 : %p\n",msg_recv);
                ESP_LOGI(TAG, "Mac received: %02X:%02X:%02X:%02X:%02X:%02X", msg_recv->mac_recv[0], msg_recv->mac_recv[1], msg_recv->mac_recv[2], msg_recv->mac_recv[3], msg_recv->mac_recv[4], msg_recv->mac_recv[5]);
                /* Parse data, send to a queue and check the error */
                ESP_ERROR_CHECK(f_parser_data(msg_recv->msb_byte, msg_recv->lsb_byte, msg_recv->mac_recv));
            }
        }
    }

    /* Delete if something went wrong */
    vTaskDelete(NULL);
}

void task_event_handler(void *pvParameter)
{
    char TAG[] = "TASK EVENT";
    ESP_LOGI(TAG, "Event handler task initialized");

    event_t evt;
    event_t *evt_p = &evt;

    /* Queues creates for every event */
    esp_err_t ret_queues = f_queues_for_events();
    if (ret_queues == ESP_OK)
    {
        ESP_LOGI(TAG, "Queue creation was successfull");
    }

    /* Tasks creates for every event */
    esp_err_t ret_tasks = f_event_task_creates();
    if (ret_tasks == ESP_OK)
    {
        ESP_LOGI(TAG, "Tasks creation was successfull");
    }

    while (true)
    {
        if (evt_queue != NULL)
        {
            if (xQueueReceive(evt_queue, evt_p, portMAX_DELAY) == pdTRUE)
            {
                //printf("Evento: %i,%i\n", evt.evt_rcv, evt.evt_det);
                f_task_dispatcher(&evt);
            }
        }
    }

    /* Delete if something went wrong */
    vTaskDelete(NULL);
}

void task_sync_by_mac(void *pvParameter)
{
    ESP_LOGI("TASK SYNC", "Task initialized");

    /* Casting param */
    uint8_t *mac = ((uint8_t *)pvParameter);

    if (f_sync_new_device(mac) == ESP_OK)
    {
        ESP_LOGI("TASK SYNC", "New device synchronized");
        ESP_LOGI("TASK SYNC", "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    else
    {
        ESP_LOGE("TASK SYNC", "ERROR");
    }

    /* Free memory. I don't need it anymore */
    vPortFree(mac);
    vTaskDelete(NULL);
}

void command_task(void *pvParameter)
{
    char TAG[] = "COMMAND TASK";
    ESP_LOGI(TAG, "Created");

    uint8_t command = 0;

    while (true)
    {
        if (xQueueReceive(command_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            //printf("RECIBI EL COMANDO: %02X\n", command);
            switch (command)
            {
            case LEFT_ON:
            {
                f_delete_tasks();
                BaseType_t ret1 = xTaskCreate(f_left_lights, "Left", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xLeftHandle);
                if (ret1 != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;
            case RIGHT_ON:
            {
                f_delete_tasks();
                BaseType_t ret2 = xTaskCreate(f_right_lights, "Right", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xRightHandle);
                if (ret2 != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;
            case TEST_ON:
            {
                f_delete_tasks();
                BaseType_t ret3 = xTaskCreate(f_test_lights, "Test", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xTestHandle);
                if (ret3 != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;

            case LEFT_OFF:
            case RIGHT_OFF:
            case TEST_OFF:
            {
                f_delete_tasks();
            }
            break;
            default:
            {
                ESP_LOGI(TAG, "Something went wrong");
                f_delete_tasks();
            }
            break;
            }
        }
        else
        {
            ESP_LOGI(TAG, "No element on the queue");
        }
    }
}

void moving_task(void *pvParameter)
{
    char TAG[] = "MOVING TASK";
    ESP_LOGI(TAG, "Created");

    uint8_t command = 0;

    while (true)
    {
        if (xQueueReceive(moving_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            switch (command)
            {
            case MOVING:
            {
                f_delete_tasks();
                BaseType_t ret = xTaskCreate(f_moving, "Moving", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xMovingHandle);
                if (ret != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;
            case STOPPED:
            {
                f_delete_tasks();
                BaseType_t ret1 = xTaskCreate(f_stopped, "Stopped", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xStoppedHandle);
                if (ret1 != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;
            }
        }
        else
        {
            //ESP_LOGI(TAG, "No element on the queue");
        }
    }
}
void turn_task(void *pvParameter)
{
    char TAG[] = "TURN TASK";
    ESP_LOGI(TAG, "Created");

    uint8_t command = 0;
    while (true)
    {
        if (xQueueReceive(turn_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            switch (command)
            {
            case TURN_RIGH:
            {
                f_delete_tasks();
                BaseType_t ret = xTaskCreate(f_turn_right, "Turn Right", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xTurnRightHandle);
                if (ret != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;
            case TURN_LEFT:
            {
                f_delete_tasks();
                BaseType_t ret1 = xTaskCreate(f_turn_left, "Turn Left", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xTurnLeftHandle);
                if (ret1 != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
            break;
            }
        }
        else
        {
            //ESP_LOGI(TAG, "No element on the queue");
        }
    }
}
void emergency_task(void *pvParameter)
{
    char TAG[] = "EMERGENCY TASK";
    ESP_LOGI(TAG, "Created");

    uint8_t command = 0;

    while (true)
    {
        if (xQueueReceive(emergency_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            if (command == EMERGENCY_D)
            {
                f_delete_tasks();
                ESP_LOGI(TAG, "EMERGENCY TRIGGERED");
                BaseType_t ret = xTaskCreate(f_emergency, "Emergency", 2 * configMINIMAL_STACK_SIZE, NULL, 3, &xEmergencyHandle);
                if (ret != pdPASS)
                {
                    ESP_LOGE(TAG, "Error creating task");
                }
            }
        }
        else
        {
            //ESP_LOGI(TAG, "No element on the queue");
        }
    }
}

void f_delete_tasks(void)
{
    if (xTestHandle != NULL)
    {
        vTaskDelete(xTestHandle);
        xTestHandle = NULL;
        //printf("Tarea test borrada\n");
    }

    if (xLeftHandle != NULL)
    {
        vTaskDelete(xLeftHandle);
        xLeftHandle = NULL;
        //printf("Tarea left borrada\n");
    }
    if (xRightHandle != NULL)
    {
        vTaskDelete(xRightHandle);
        xRightHandle = NULL;
        //printf("Tarea right borrada\n");
    }
    if (xMovingHandle != NULL)
    {
        vTaskDelete(xMovingHandle);
        xMovingHandle = NULL;
        //printf("Tarea moving borrada\n");
    }
    if (xEmergencyHandle != NULL)
    {
        vTaskDelete(xEmergencyHandle);
        xEmergencyHandle = NULL;
        //printf("Tarea right borrada\n");
    }
    if (xTurnRightHandle != NULL)
    {
        vTaskDelete(xTurnRightHandle);
        xTurnRightHandle = NULL;
        //printf("Tarea right borrada\n");
    }
    if (xTurnLeftHandle != NULL)
    {
        vTaskDelete(xTurnLeftHandle);
        xTurnLeftHandle = NULL;
        //printf("Tarea left borrada\n");
    }

    if (xStoppedHandle != NULL)
    {
        vTaskDelete(xStoppedHandle);
        xStoppedHandle = NULL;
        //printf("Tarea left borrada\n");
    }
}