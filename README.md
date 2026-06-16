# WS2812B 16x16 Matrix Library for STM32

A clean and well-organized WS2812B driver with animations for STM32 microcontrollers (tested on F411RE).  
Project generated in STM32CubeMX and edited in VS Code  


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

## Quick Start
Add this to CMakeList.txt  

```text
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Core driver and animations
    Core/Src/ws2812b_driver.c
    Core/Src/ws2812b_animations.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Inc
)
```

## Building
Use VS Code + CMake Tools.  
Run CMake: Clean Rebuild after changes.  

main.c  
```c
#include "ws2812b.h"

int main(void)
{
    // ... System and peripheral initialization ...

    while (1)
    {
        Set_Matrix_Rainbow(5, 30);        // Beautiful rotating rainbow
        // LED_Test_Pattern(5, 50);
        // Set_Random_Flash(3, 60);
        // Set_Matrix_Fire(25, 35);
    }
}
```
## Notes

Brightness above 10-15 can be very bright — start low.  
The library is designed to be easy to expand with new animation files.


Author: Troy  
GitHub: TroyHans
