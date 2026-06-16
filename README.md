# WS2812B 16x16 Matrix Library for STM32

A clean and well-organized WS2812B driver with animations for STM32 microcontrollers (tested on F411RE).

## Features

- Natural **X,Y coordinates** (0,0 = Top-Left)
- Correct serpentine mapping for standard 16x16 WS2812B matrices (LED 0 at Top-Right)
- DMA + Timer PWM driver for smooth performance
- Brightness control (0-45)
- Multiple built-in animations
- Easy to extend and reuse

## Hardware Requirements

- STM32F411RE (or compatible)
- 16x16 WS2812B LED Matrix
- TIM1 Channel 1 (PA8) with DMA
- Recommended: 390Ω resistor on data line + 22pF capacitor to ground
- Stable 5V power supply with proper bypassing

## Project Structure
Core/  
├── Inc/  
│   └── ws2812b.h  
├── Src/  
│   ├── ws2812b_driver.c  
│   ├── ws2812b_animations.c  
│   └── main.c


## Available Functions
## Core Driver

SetPixel(x, y, r, g, b) — Set pixel using natural coordinates  
Set_Brightness(brightness) — Set global brightness (0-45)  
Set_All_LEDs(r, g, b, brightness, delay_ms)  
WS2812_Send()

## Animations

Set_Matrix_Rainbow(brightness, speed_ms)  
LED_Test_Pattern(brightness, delay_ms) — Column sweep with color cycling  
Set_Random_Flash(brightness, delay_ms)  
Set_Matrix_Fire(intensity, delay_ms)

## Building
Use VS Code + CMake Tools.  
Run CMake: Clean Rebuild after changes.

## Notes

Brightness above 10-15 can be very bright — start low.  
The library is designed to be easy to expand with new animation files.


Author: Troy  
GitHub: TroyHans
