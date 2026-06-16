#ifndef WS2812B_H
#define WS2812B_H

#include <stdint.h>

#define MAX_LED          256
#define USE_BRIGHTNESS   1

// Driver Functions
void Set_LED(int LEDnum, int Red, int Green, int Blue);
void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void Set_Brightness(int brightness);
void WS2812_Send(void);
void Set_All_LEDs(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint16_t delay_ms);

// Animation Functions
void LED_Test_Pattern(uint8_t brightness, uint16_t delay_ms);
void Set_Matrix_Rainbow(uint8_t brightness, uint16_t speed_ms);
void Set_Random_Flash(uint8_t brightness, uint16_t delay_ms);
void Set_Matrix_Fire(uint8_t intensity, uint16_t delay_ms);

#endif