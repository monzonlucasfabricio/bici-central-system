#include "include/tasks.h"
#include "../../components/events/include/events_dispatch.h"

void f_time_to_delete_task(void);
void flag_to_delete(void);
void flag_to_restore(void);

void task_recv_msg(void *pvParameter)
{
    char TAG[] = "RECEIVE_TASK";
    ESP_LOGI(TAG, "INITIALIZED");
    msg_t msg;
    msg_t *msg_recv = &msg;

    while (true)
    {
        if (rcv_queue != NULL)
        {
            if (xQueueReceive(rcv_queue, msg_recv, portMAX_DELAY) == pdTRUE)
            {
                /* Log MAC information */
                ESP_LOGI(TAG, "MAC RECEIVED: %02X:%02X:%02X:%02X:%02X:%02X", msg_recv->mac_recv[0], msg_recv->mac_recv[1], msg_recv->mac_recv[2], msg_recv->mac_recv[3], msg_recv->mac_recv[4], msg_recv->mac_recv[5]);
                
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
    ESP_LOGI("EVENT HANDLER TASK", "INITIALIZED");

    event_t evt;
    event_t *evt_p = &evt;

    while (true)
    {
        if (evt_queue != NULL)
        {
            if (xQueueReceive(evt_queue, evt_p, portMAX_DELAY) == pdTRUE)
            {
                /* Send to the respect QUEUE*/
                f_task_dispatcher(&evt);
            }
        }
    }

    /* Delete if something went wrong */
    vTaskDelete(NULL);
}

void task_sync_by_mac(void *pvParameter)
{
    char TAG[] = "TASK SYNC";
    ESP_LOGI(TAG, "Task initialized");

    /* Casting param */
    uint8_t *mac = ((uint8_t *)pvParameter);

    if (f_sync_new_device(mac) == ESP_OK)
    {
        ESP_LOGI(TAG, "New device synchronized");
        ESP_LOGI(TAG, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    else
    {
        ESP_LOGE(TAG, "ERROR");
    }

    /* Free memory. I don't need it anymore */
    vPortFree(mac);
    vTaskDelete(NULL);
}

void command_task(void *pvParameter)
{
    char TAG[] = "COMMAND TASK";
    ESP_LOGI(TAG, "CREATED");

    uint8_t command = 0;

    while (true)
    {
        if (xQueueReceive(command_queue, &command, portMAX_DELAY) == pdTRUE)
        {

            f_time_to_delete_task();

            switch (command)
            {
            case LEFT_ON:
            {
                ESP_LOGI(TAG, "CHANGE TO LEFT -> ON");
                xExecutingTaskHandler = xTaskCreateStatic(f_left_lights, 
                                                "Left", 
                                                STACK_SIZE,
                                                NULL,
                                                EVENT_TASK_PRIORITY,
                                                xStack,
                                                &xTaskBuffer);
            }
            break;
            case RIGHT_ON:
            {
                ESP_LOGI(TAG, "CHANGE TO RIGHT -> ON");
                xExecutingTaskHandler = xTaskCreateStatic(f_right_lights, 
                                                "Right", 
                                                STACK_SIZE,
                                                NULL,
                                                EVENT_TASK_PRIORITY,
                                                xStack,
                                                &xTaskBuffer);
            }
            break;
            case TEST_ON:
            {
                ESP_LOGI(TAG, "CHANGE TO TEST -> ON");
                xExecutingTaskHandler = xTaskCreateStatic(f_test_lights, 
                                                "Test", 
                                                STACK_SIZE,
                                                NULL,
                                                EVENT_TASK_PRIORITY,
                                                xStack,
                                                &xTaskBuffer);
            }
            break;

            case LEFT_OFF:
            case RIGHT_OFF:
            case TEST_OFF:
            {
                ESP_LOGI(TAG, "CHANGE TO LEFT,RIGHT OR TEST -> OFF");
                f_time_to_delete_task();
            }
            break;
            default:
            {
                ESP_LOGE(TAG, "SOMETHING WENT WRONG");
            }
            break;
            }
        }
        else
        {
            ESP_LOGE(TAG, "THERE IS NO ELEMENT ON THE QUEUE");
        }
    }
}

void moving_task(void *pvParameter)
{
    char TAG[] = "MOVING TASK";
    ESP_LOGI(TAG, "CREATED");

    uint8_t command = 0;

    while (true)
    {
        if (xQueueReceive(moving_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            f_time_to_delete_task();
            switch (command)
            {
            case MOVING:
            {
                ESP_LOGI(TAG, "CHANGE TO MOVING -> ON");
                xExecutingTaskHandler = xTaskCreateStatic(f_moving, 
                                                "Moving", 
                                                STACK_SIZE,
                                                NULL,
                                                EVENT_TASK_PRIORITY,
                                                xStack,
                                                &xTaskBuffer);
            }
            break;
            case STOPPED:
            {
                ESP_LOGI(TAG, "CHANGE TO STOPPED -> ON");
                xExecutingTaskHandler = xTaskCreateStatic(f_stopped, 
                                                "Stopped", 
                                                STACK_SIZE,
                                                NULL,
                                                EVENT_TASK_PRIORITY,
                                                xStack,
                                                &xTaskBuffer);
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
    ESP_LOGI(TAG, "CREATED");

    uint8_t command = 0;
    while (true)
    {
        if (xQueueReceive(turn_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            f_time_to_delete_task();

            switch (command)
            {
            case TURN_RIGH:
            {
                ESP_LOGI(TAG, "CHANGE TO TURN RIGHT");
                xExecutingTaskHandler = xTaskCreateStatic(f_turn_right, 
                                                    "Turn_right", 
                                                    STACK_SIZE,
                                                    NULL,
                                                    EVENT_TASK_PRIORITY,
                                                    xStack,
                                                    &xTaskBuffer);
            }
            break;
            case TURN_LEFT:
            {
                ESP_LOGI(TAG, "CHANGE TO TURN LEFT");
                xExecutingTaskHandler = xTaskCreateStatic(f_turn_left, 
                                                    "Moving", 
                                                    STACK_SIZE,
                                                    NULL,
                                                    EVENT_TASK_PRIORITY,
                                                    xStack,
                                                    &xTaskBuffer);
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
    ESP_LOGI(TAG, "CREATED");

    uint8_t command = 0;

    while (true)
    {
        if (xQueueReceive(emergency_queue, &command, portMAX_DELAY) == pdTRUE)
        {
            f_time_to_delete_task();

            if (command == EMERGENCY_D)
            {
                ESP_LOGI(TAG, "CHANGE TO EMERGENCY -> ON");
                xExecutingTaskHandler = xTaskCreateStatic(f_emergency, 
                                                    "Emergency", 
                                                    STACK_SIZE,
                                                    NULL,
                                                    EVENT_TASK_PRIORITY,
                                                    xStack,
                                                    &xTaskBuffer);
            }
        }
        else
        {
            //ESP_LOGI(TAG, "No element on the queue");
        }
    }
}

void flag_to_delete(void)
{
    if (xSemaphoreTake(xNeoPixelFlag, portMAX_DELAY) == pdTRUE)
    {
        flags.stopped_flag = HIGH;
        xSemaphoreGive(xNeoPixelFlag);
    }
}

void flag_to_restore(void)
{
    if (xSemaphoreTake(xNeoPixelFlag, portMAX_DELAY) == pdTRUE)
    {
        flags.stopped_flag = LOW;
        xSemaphoreGive(xNeoPixelFlag);
    }
}

void f_time_to_delete_task(void){
    ESP_LOGI("DELETE TASK","DONE");
    flag_to_delete();
    vTaskDelay(TASK_TIME_TO_DELETE / portTICK_PERIOD_MS);
    flag_to_restore();
}