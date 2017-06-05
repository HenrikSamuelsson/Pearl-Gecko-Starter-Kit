
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_system.h"
#include "em_timer.h"
#include "em_chip.h"

/* Port and pin configuration for leds and buttons */
#define LED_PORT        gpioPortF
#define BUTTON_PORT     gpioPortF
#define PIN_LED_0           4
#define PIN_LED_1           5
#define PIN_PB_0            6
#define PIN_PB_1            7

uint16_t ms_counter = 0;
  
void TIMER0_IRQHandler(void)
{
  TIMER_IntClear(TIMER0, TIMER_IF_OF);
  ms_counter++;
}

int main() {
        
        /* Chip errata */
        CHIP_Init();
        
        CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_2);               // Set HF clock divider to /2 to keep core frequency <32MHz
        CMU_OscillatorEnable(cmuOsc_HFXO, true, true);           // Enable XTAL Osc and wait to stabilize
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);         // Select HF XTAL osc as system clock source. 40MHz XTAL, but we divided the system clock by 2, therefore our HF clock will be 20MHz
   
        CMU_ClockEnable(cmuClock_GPIO, true);                    // Enable GPIO peripheral clock
        CMU_ClockEnable(cmuClock_TIMER0, true);                  // Enable TIMER0 peripheral clock
   
        GPIO_PinModeSet(LED_PORT, PIN_LED_0, gpioModePushPull, 0); // Configure LED0 pin as digital output (push-pull)
        GPIO_PinOutClear(LED_PORT, PIN_LED_0);                     // Turn off LED0
   
        TIMER_TopSet(TIMER0, 20000);                             // Set timer TOP value
        TIMER_Init_TypeDef timerInit =                           // Setup Timer initialization
        {
                .enable     = true,
                .debugRun   = true,
                .prescale   = timerPrescale1,
                .clkSel     = timerClkSelHFPerClk,
                .fallAction = timerInputActionNone,
                .riseAction = timerInputActionNone,
                .mode       = timerModeUp,
                .dmaClrAct  = false,
                .quadModeX4 = false,
                .oneShot    = false,
                .sync       = false,
        };
        TIMER_IntEnable(TIMER0, TIMER_IF_OF);     // Enable Timer0 overflow interrupt
        NVIC_EnableIRQ(TIMER0_IRQn);              // Enable TIMER0 interrupt vector in NVIC
        TIMER_Init(TIMER0, &timerInit);           // Configure and start Timer0
           
        while(1) {
                if(ms_counter == 500) {
                        GPIO_PinOutToggle(LED_PORT, PIN_LED_0); // Toggle LED
                        ms_counter = 0;                         // Reset counter
                }
        }
}