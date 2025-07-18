#include "stepper.h"



#define PIN 0
#define LED_PIN 1

void setupStruct(STEPPER_DRV * stepper){
    stepper->StepsToTake = 0;
    stepper->activedir = true;
    stepper->dirchange = true;
    stepper->ActiveAngle = 0.0;
    stepper->directionchangeDelayCounter = 0;
    stepper->stm_pio = pio0;
}

void setupPIO(STEPPER_DRV * stepper)
{
    // stepper 1 pio
    uint stm_offset = pio_add_program(stepper->stm_pio, &stepper_precise_program);
    stepper_1_program_init(stepper->stm_pio, stepper->stm_sm, stm_offset, stepper->STEP_PIN, 1000, true);
}

void setupGPIO(STEPPER_DRV * stepper)
{

    stepper->DIR_PIN = 17;
    stepper->stm_sm = 1;
    stepper->STEP_PIN = 0;
    stepper->FULLSTEP = 800;
    // stepper enable pin
    gpio_init(PIN);
    gpio_set_dir(PIN, GPIO_OUT);
    // gpio_put(STM_EN, 0);

    // stepperMotor1 dir pin
    gpio_init(stepper->DIR_PIN);
    gpio_set_dir(stepper->DIR_PIN, GPIO_OUT);
    // gpio_put(stepperMotor1.DIR_PIN, 1);

    // inbuilt led
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}