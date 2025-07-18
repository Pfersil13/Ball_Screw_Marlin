#ifndef STEPPER_H
    #define STEPPER_H

        #include <stdio.h>
        #include <stdlib.h>
        #include "pico/stdlib.h"
        #include "hardware/dma.h"
        #include "hardware/pio.h"
        #include "hardware/timer.h"
        #include "hardware/clocks.h"
        #include "hardware/uart.h"
        #include "stepper.pio.h"
        
        #define LEFT -1
        #define RIGHT 1

        #define BUFFER_SIZE 100
        
        typedef struct {
            int StepsToTake;
            bool activedir;
            int dir;
            float ActiveAngle; 
            int DIR_PIN;
            int STEP_PIN;
            int ENABLE_PIN;
            int HOME_PIN;
            int FULLSTEP;
            int directionchangeDelayCounter;
            PIO stm_pio;
            int stm_sm;
            int buffer[2][BUFFER_SIZE];
        } STEPPER_DRV;


        void setupGPIO(STEPPER_DRV * stepper);
        void setupStepper(STEPPER_DRV * stepper, uint8_t enable_pin, uint8_t dir_pin, uint8_t step_pin);
        void setupPIO(STEPPER_DRV * stepper);
        void pio0_irq0_handler();

        void setFrec( STEPPER_DRV * stepper,float desired_freq);
        void moveSteps( STEPPER_DRV * stepper, int steps );
        void setDirection( STEPPER_DRV * stepper,int direction);


        
#endif