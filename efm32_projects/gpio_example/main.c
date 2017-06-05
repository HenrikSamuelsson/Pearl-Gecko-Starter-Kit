/**
 * @file mainc.c
 *
 * @author 
 *      Henrik Samuelsson
 *
 * @brief 
 *      Lets a user control leds on a Pearl Gecko devolopment kit.
 *      
 * @details 
 *      Project is based on code from the following web page
 *      https://eewiki.net/display/microcontroller/Getting+Started+with+EFM32+Giant+Gecko+ARM+Cortex-M3
 *
 * @details 
 *      The development kit has two leds and two push buttons that are connected
 *      in the following way
 *
 *      PF4     LED0
 *      PF5     LED1
 *      PF6     PUSH_BUTTON0
 *      PF7     PUSH_BUTTON1
 */
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_system.h"
#include "em_chip.h"    // required for CHIP_Init() function

#define LED_PORT gpioPortF
#define BUTTON_PORT gpioPortF
#define LED0 4
#define LED1 5
#define PB0 6
#define PB1 7

int main() {
        
        /* Chip errata */
        CHIP_Init();
        
      CMU_ClockEnable(cmuClock_GPIO, true);                      // Enable GPIO peripheral clock
   
  GPIO_PinModeSet(LED_PORT, LED0, gpioModePushPull, 0);      // Configure LED0 pin as digital output (push-pull)
  GPIO_PinModeSet(LED_PORT, LED1, gpioModePushPull, 1); // Configure LED1 pin as digital output (push-pull) with drive-strength to lowest setting
  //GPIO_DriveModeSet(LED_PORT, gpioDriveModeLowest);          // Set DRIVEMODE to lowest setting (0.5 mA) for all LEDs configured with alternate drive strength
  GPIO_PinModeSet(BUTTON_PORT, PB0, gpioModeInputPull, 1);   // Configure PB0 as input with pull-up enabled
  GPIO_PinModeSet(BUTTON_PORT, PB1, gpioModeInputPull, 1);   // Configure PB1 as input with pull-up enabled
   
  while(1) {
    if(! (GPIO_PinInGet(BUTTON_PORT, PB0)) ) {               // If PB0 is pressed
      GPIO_PinOutSet(LED_PORT, LED1);                        // Turn on LED1
    }else{
      GPIO_PinOutClear(LED_PORT, LED1);                      // Turn off LED1
    }
    if(! (GPIO_PinInGet(BUTTON_PORT, PB1)) ) {               // If PB1 is pressed
      GPIO_PinOutClear(LED_PORT, LED0);                      // Turn off LED0
    }else{
      GPIO_PinOutSet(LED_PORT, LED0);                        // Turn on LED0
    }
  }
}