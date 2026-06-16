#include "ws2812b.h"
#include "main.h"
#include "tim.h"
#include "dma.h"

// Global variables
uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];
uint8_t datasentflag = 0;
uint16_t pwmData[(24*MAX_LED)+50];

void Set_LED(int LEDnum, int Red, int Green, int Blue)
{
    if(LEDnum >= MAX_LED) return;
    
    LED_Data[LEDnum][0] = LEDnum;
    LED_Data[LEDnum][1] = Green;
    LED_Data[LEDnum][2] = Red;
    LED_Data[LEDnum][3] = Blue;
}

void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x >= 16 || y >= 16) return;

    uint32_t led_num = (uint32_t)y * 16;

    if (y % 2 == 0) {
        led_num += (15 - x);   // Even rows: right to left
    } else {
        led_num += x;          // Odd rows: left to right
    }

    Set_LED(led_num, r, g, b);
}

void Set_Brightness(int brightness)
{
#if USE_BRIGHTNESS
    if (brightness > 45) brightness = 45;
    if (brightness < 0)  brightness = 0;

    float scale = brightness / 45.0f;

    for (int i = 0; i < MAX_LED; i++)
    {
        LED_Mod[i][0] = LED_Data[i][0];
        LED_Mod[i][1] = (uint8_t)(LED_Data[i][1] * scale);
        LED_Mod[i][2] = (uint8_t)(LED_Data[i][2] * scale);
        LED_Mod[i][3] = (uint8_t)(LED_Data[i][3] * scale);
    }
#endif
}

void WS2812_Send(void)
{
    uint32_t indx = 0;
    uint32_t color;

    for (uint32_t i = 0; i < MAX_LED; i++)
    {
#if USE_BRIGHTNESS
        color = ((LED_Mod[i][1] << 16) | (LED_Mod[i][2] << 8) | LED_Mod[i][3]);
#else
        color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | LED_Data[i][3]);
#endif

        for (int8_t bit = 23; bit >= 0; bit--)
        {
            if (color & (1u << bit))
                pwmData[indx] = 60;
            else
                pwmData[indx] = 30;

            indx++;
        }
    }

    for (uint8_t i = 0; i < 50; i++)
    {
        pwmData[indx++] = 0;
    }

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);

    while (!datasentflag) {}
    datasentflag = 0;
}

void Set_All_LEDs(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint16_t delay_ms)
{
    for(uint32_t i = 0; i < MAX_LED; i++)
    {
        Set_LED(i, red, green, blue);
    }
    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
        datasentflag = 1;
    }
}