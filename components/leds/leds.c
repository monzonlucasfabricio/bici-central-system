/**************************************************************************************************
 ** (c) Copyright 2019: NAME <EMAIL>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file leds.c
 ** @brief Leds library. Just to show how to organize the files.
 **
 **| REV | YYYY.MM.DD | Autor           | Changes                                                 |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2021.01.12 | lucasmonzonl    | Initial version                                         |
 **
 ** @addtogroup leds
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "include/leds.h"

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */
uint8_t internaldelay_n_check(uint32_t delayms, uint8_t *flagtocheck);
/* === Definiciones de funciones internas ====================================================== */

/* === Definiciones de funciones externas ====================================================== */
esp_err_t f_neo_leds_init(pixel_settings_t *px)
{
    uint32_t pixels[NR_LED];
    int i;
    int rc;

    rc = neopixel_init(NEOPIXEL_PORT, NEOPIXEL_RMT_CHANNEL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    for (i = 0; i < NR_LED; i++)
    {
        pixels[i] = 0;
    }
    px->pixels = (uint8_t *)pixels;
    px->pixel_count = NR_LED;

#ifdef NEOPIXEL_WS2812
    strcpy(px->color_order, "GRB");
#else
    strcpy(px->color_order, "GRBW");
#endif
    memset(&px->timings, 0, sizeof(pixel_timing_t));
    px->timings.mark.level0 = 1;
    px->timings.space.level0 = 1;
    px->timings.mark.duration0 = 12;

#ifdef NEOPIXEL_WS2812
    px->nbits = 24;
    px->timings.mark.duration1 = 14;
    px->timings.space.duration0 = 7;
    px->timings.space.duration1 = 16;
    px->timings.reset.duration0 = 600;
    px->timings.reset.duration1 = 600;
#endif
#ifdef NEOPIXEL_SK6812
    p->nbits = 32;
    px->timings.mark.duration1 = 12;
    px->timings.space.duration0 = 6;
    px->timings.space.duration1 = 18;
    px->timings.reset.duration0 = 900;
    px->timings.reset.duration1 = 900;
#endif

    px->brightness = 0x80;
    np_show(px, NEOPIXEL_RMT_CHANNEL);

    return ESP_OK;
}

esp_err_t f_create_led(uint8_t pinled, modes_s mode, ledsparams_t *led)
{
    ledsparams_t newled = {
        .pinLed = pinled,
        .mode = mode,
    };

    memcpy(led, &newled, sizeof(ledsparams_t));

    return ESP_OK;
}

/* ----------- LEDS TASK SECTION -------------------------------------*/
/* This will change location to tasks.c */

void f_test_lights(void *pvParameter)
{
    ESP_LOGI("TEST","INIT");

    while (true)
    {
        printf("------------> TEST ON\n");
        np_set_pixel_rgbw(&px, 2, 100, 100, 100, 0);
        np_set_pixel_rgbw(&px, 1, 100, 100, 100, 0);
        np_set_pixel_rgbw(&px, 0, 100, 100, 100, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
            np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
            np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
        np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
        np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}
void f_left_lights(void *pvParameter)
{
    ESP_LOGI("LEFT","INIT");
    np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);

    while (true)
    {
        printf("------------> LEDS LEFT ON\n");
        np_set_pixel_rgbw(&px, 1, 100, 100, 0, 0);
        np_set_pixel_rgbw(&px, 2, 100, 100, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
            np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
        np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}
void f_right_lights(void *pvParameter)
{
    ESP_LOGI("RIGHT","INIT");
    np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);

    while (true)
    {
        printf("------------> LEDS RIGHT ON\n");
        np_set_pixel_rgbw(&px, 1, 100, 100, 0, 0);
        np_set_pixel_rgbw(&px, 0, 100, 100, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
            np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
        np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}

void f_moving(void *pvParameter)
{
    ESP_LOGI("MOVING","INIT");
    np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
    np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
    np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);

    np_show(&px, NEOPIXEL_RMT_CHANNEL);

    while (true)
    {
        printf("------------> Moving ON\n");
        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}

void f_turn_right(void *pvParameter)
{
    ESP_LOGI("TURN RIGHT","INIT");
    np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
    np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);

    while (true)
    {
        printf("------------> TURN RIGHT ON\n");
        np_set_pixel_rgbw(&px, 0, 100, 100, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(200, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(200, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}

void f_turn_left(void *pvParameter)
{
    ESP_LOGI("TURN LEFT","INIT");
    np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
    np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);

    while (true)
    {
        printf("------------> TURN LEFT ON\n");
        np_set_pixel_rgbw(&px, 2, 100, 100, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(200, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(200, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}

void f_emergency(void *pvParameter)
{
    ESP_LOGI("EMERGENCY","INIT");

    while (true)
    {
        printf("------------> EMERGENCY ON\n");
        np_set_pixel_rgbw(&px, 2, 100, 50, 100, 0);
        np_set_pixel_rgbw(&px, 1, 100, 50, 100, 0);
        np_set_pixel_rgbw(&px, 0, 100, 50, 100, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
            np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
            np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);
        np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
        np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}

void f_stopped(void *pvParameter)
{
    ESP_LOGI("STOPPED","INIT");
    np_set_pixel_rgbw(&px, 0, 0, 0, 0, 0);
    np_set_pixel_rgbw(&px, 2, 0, 0, 0, 0);

    while (true)
    {
        printf("------------> STOPPED ONc\n");
        np_set_pixel_rgbw(&px, 1, 200, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
            np_show(&px, NEOPIXEL_RMT_CHANNEL);
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }

        np_set_pixel_rgbw(&px, 1, 0, 0, 0, 0);
        np_show(&px, NEOPIXEL_RMT_CHANNEL);

        if (internaldelay_n_check(100, &flags.stopped_flag) == HIGH)
        {
            xSemaphoreGive(xNeoPixelFlag);
            vTaskDelete(NULL);
        }
    }
}

uint8_t internaldelay_n_check(uint32_t delayms, uint8_t *flagtocheck)
{
    for (uint8_t i = 0; i < (delayms / 10); i++)
    {
        if (xSemaphoreTake(xNeoPixelFlag, 1 / portTICK_PERIOD_MS) == pdTRUE)
        {
            if (*flagtocheck == HIGH)
            {
                return HIGH;
            }
        }
        xSemaphoreGive(xNeoPixelFlag);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    return LOW;
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */