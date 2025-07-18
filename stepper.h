#ifndef STEPPER_H
    #define STEPPER_H

        #include <stdio.h>
        #include "pico/stdlib.h"
        #include "hardware/dma.h"
        #include "hardware/pio.h"
        #include "hardware/timer.h"
        #include "hardware/clocks.h"
        #include "hardware/uart.h"
        #include "stepper.pio.h"

        typedef struct {
            int StepsToTake;
            bool activedir;
            bool dirchange;
            float ActiveAngle; // 360/res
            int DIR_PIN;
            int STEP_PIN;
            int HOME_PIN;
            int FULLSTEP;
            int directionchangeDelayCounter;
            PIO stm_pio;
            int stm_sm;
        } STEPPER_DRV;


        void setupGPIO(STEPPER_DRV * stepper);
        void setupStruct(STEPPER_DRV * stepper);
        void setupPIO(STEPPER_DRV * stepper);
        
#endif