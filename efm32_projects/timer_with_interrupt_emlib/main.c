
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_system.h"
#include "em_timer.h"
#include "em_chip.h"

#define LED_PORT gpioPortF
#define BUTTON_PORT     gpioPortF
#define LED_0           4
#define LED_1           5
#define PB_0            6
#define PB_1            7

uint16_t ms_counter = 0;
  
void TIMER0_IRQHandler(void)
{
  TIMER_IntClear(TIMER0, TIMER_IF_OF);      // Clear overflow flag
  ms_counter++;                             // Increment counter
}

int main() {
        
        /* Chip errata */
        CHIP_Init();
        
        CMU_HFRCOBandSet(1000000U);      // Set High Freq. RC Osc. to 1 MHz
        CMU_ClockEnable(cmuClock_GPIO, true);     // Enable GPIO peripheral clock
        CMU_ClockEnable(cmuClock_TIMER0, true);   // Enable TIMER0 peripheral clock
  
        GPIO_PinModeSet(LED_PORT, LED_0, gpioModePushPull, 0); // Configure LED0 pin as digital output (push-pull)
    
  TIMER_TopSet(TIMER0, 1000);               // Set timer TOP value
  TIMER_Init_TypeDef timerInit =            // Setup Timer initialization
  {
    .enable     = true,                     // Start timer upon configuration
    .debugRun   = true,                     // Keep timer running even on debug halt
    .prescale   = timerPrescale1,           // Use /1 prescaler...timer clock = HF clock = 1 MHz
    .clkSel     = timerClkSelHFPerClk,      // Set HF peripheral clock as clock source
    .fallAction = timerInputActionNone,     // No action on falling edge
    .riseAction = timerInputActionNone,     // No action on rising edge
    .mode       = timerModeUp,              // Use up-count mode
    .dmaClrAct  = false,                    // Not using DMA
    .quadModeX4 = false,                    // Not using quad decoder
    .oneShot    = false,                    // Using continuous, not one-shot
    .sync       = false,                    // Not synchronizing timer operation off of other timers
  };
  TIMER_IntEnable(TIMER0, TIMER_IF_OF);     // Enable Timer0 overflow interrupt
  NVIC_EnableIRQ(TIMER0_IRQn);              // Enable TIMER0 interrupt vector in NVIC
  TIMER_Init(TIMER0, &timerInit);           // Configure and start Timer0
  
  while(1)
  {
    if(ms_counter == 500) {
      GPIO_PinOutToggle(LED_PORT, LED_0); // Toggle LED
      ms_counter = 0;                       // Reset counter
    }
  }
}