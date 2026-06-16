/* ==================== ws2812b_animations.c ==================== */
#include "ws2812b.h"
#include "main.h"
#include "tim.h"
#include <stdlib.h>

/*====================== Animation Functions ======================*/

void LED_Test_Pattern(uint8_t brightness, uint16_t delay_ms)
{
    static uint8_t current_col = 0;
    static int8_t direction = 1;
    static uint8_t color_phase = 0;

    // Clear all LEDs
    for(uint32_t i = 0; i < MAX_LED; i++) {
        Set_LED(i, 0, 0, 0);
    }

    // Set current color
    uint8_t r = 0, g = 0, b = 0;
    if (color_phase == 0)      r = 255;
    else if (color_phase == 1) g = 255;
    else                       b = 255;

    // Light full column
    for(uint8_t y = 0; y < 16; y++) {
        SetPixel(current_col, y, r, g, b);
    }

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);

    // Move column
    current_col += direction;

    if (current_col >= 15) {
        current_col = 15;
        direction = -1;
    } 
    else if (current_col <= 0) {
        current_col = 0;
        direction = 1;
        color_phase = (color_phase + 1) % 3;   // Next color
    }
}

void Set_Matrix_Rainbow(uint8_t brightness, uint16_t speed_ms)
{
    static uint8_t offset = 0;

    for(uint8_t y = 0; y < 16; y++)
    {
        for(uint8_t x = 0; x < 16; x++)
        {
            uint8_t hue = (x * 16) + (y * 8) + offset;

            uint8_t r, g, b;
            uint8_t pos = hue % 256;

            if(pos < 85)
            {
                r = pos * 3;
                g = 255 - pos * 3;
                b = 0;
            }
            else if(pos < 170)
            {
                pos -= 85;
                r = 255 - pos * 3;
                g = 0;
                b = pos * 3;
            }
            else
            {
                pos -= 170;
                r = 0;
                g = pos * 3;
                b = 255 - pos * 3;
            }

            SetPixel(x, y, r, g, b);
        }
    }

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(speed_ms);

    offset += 4;          // Adjust for rotation speed
}

void Set_Random_Flash(uint8_t brightness, uint16_t delay_ms)
{
    // Clear first
    for(uint32_t i = 0; i < MAX_LED; i++) {
        Set_LED(i, 0, 0, 0);
    }

    // Create random vibrant flashes
    for(int i = 0; i < MAX_LED; i++)
    {
        if (rand() % 100 < 40)        // Chance to flash
        {
            uint8_t intensity = 140 + (rand() % 116);

            uint8_t r, g, b;

            switch(rand() % 8)
            {
                case 0:  r = intensity; g = 0;         b = 0;         break;
                case 1:  r = 0;         g = intensity; b = 0;         break;
                case 2:  r = 0;         g = 0;         b = intensity; break;
                case 3:  r = intensity; g = intensity; b = 0;         break;
                case 4:  r = intensity; g = 0;         b = intensity; break;
                case 5:  r = 0;         g = intensity; b = intensity; break;
                case 6:  r = intensity; g = intensity/2; b = 0;       break;
                default: r = intensity; g = intensity; b = intensity; break;
            }

            Set_LED(i, r, g, b);
        }
    }

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

void Set_Matrix_Fire(uint8_t intensity, uint16_t delay_ms)
{
    static uint8_t heat[16][16];

    // 1. Cool down every pixel (stronger cooling at the top)
    for(uint8_t y = 0; y < 16; y++)
    {
        for(uint8_t x = 0; x < 16; x++)
        {
            uint8_t cooling = 12 + (y / 3);           // More cooling near the top

            if (heat[y][x] > cooling)
                heat[y][x] -= cooling;
            else
                heat[y][x] = 0;
        }
    }

    // 2. Random sparks at the very bottom
    for(uint8_t x = 0; x < 16; x++)
    {
        if (rand() % 100 < 50)                        // More sparks
        {
            heat[15][x] = 180 + (rand() % 76);        // Very hot
        }
    }

    // 3. Heat rises + some sideways diffusion (makes it look wavier)
    for(uint8_t y = 0; y < 15; y++)
    {
        for(uint8_t x = 0; x < 16; x++)
        {
            uint8_t left  = heat[y+1][(x + 15) % 16];
            uint8_t right = heat[y+1][(x + 1)  % 16];

            heat[y][x] = (heat[y+1][x] + left + right) / 3;

            // Add a little randomness so it doesn't look too perfect
            if (rand() % 100 < 25)
                heat[y][x] = heat[y][x] * 90 / 100;
        }
    }

    // 4. Draw the matrix with better flame colors
    for(uint8_t y = 0; y < 16; y++)
    {
        for(uint8_t x = 0; x < 16; x++)
        {
            uint8_t temperature = heat[y][x];

            if (temperature > 200)
                SetPixel(x, y, 255, 180 + (rand()%50), 40);     // Very hot white/yellow
            else if (temperature > 140)
                SetPixel(x, y, 255, 90 + (rand()%80), 10);      // Bright orange
            else if (temperature > 80)
                SetPixel(x, y, 255, 40 + (rand()%60), 0);       // Orange/red
            else if (temperature > 30)
                SetPixel(x, y, 200, 15, 0);                     // Dark red
            else
                SetPixel(x, y, 0, 0, 0);                        // Off (important for top)
        }
    }

    Set_Brightness(intensity);
    WS2812_Send();
    HAL_Delay(delay_ms);
}
/*======================= End Animation Functions =======================*/