#include "stepper.h"

#define PIO_INTR_SM0_TXNEMPTY 0
#define PIO_INTR_IRQ0 8

#define CYCLES_PER_STEP 8.0

uint8_t steper_number = 0;
uint stm_offset;

bool flag = 0;

void setupStepper(STEPPER_DRV * stepper, uint8_t enable_pin, uint8_t dir_pin, uint8_t step_pin){

    if(steper_number > 3){
        printf("Error no hay tantos slots disponibels\n");
        return;
    }

    stepper->StepsToTake = 0;
    stepper->activedir = true;
    stepper->dir = RIGHT;
    stepper->ActiveAngle = 0.0;
    stepper->directionchangeDelayCounter = 0;
    stepper->stm_pio = pio0;
    stepper->stm_sm = steper_number;
    stepper->ENABLE_PIN = enable_pin;
    stepper->DIR_PIN = dir_pin;
    stepper->STEP_PIN = step_pin;
    stepper->FULLSTEP = 800;
    printf("%d\n", steper_number);
    steper_number++;
    
    setupGPIO(stepper);
    setupPIO(stepper);

    
}


void setupPIO(STEPPER_DRV * stepper)
{
    // stepper 1 pio
    if(flag == 0){
        stm_offset = pio_add_program(stepper->stm_pio, &stepper_precise_program);
        irq_set_exclusive_handler(PIO0_IRQ_0, pio0_irq0_handler);
        irq_set_enabled(PIO0_IRQ_0, true);

        // Activa la fuente de interrupción IRQ0 del PIO para SM0 TX (u otra fuente IRQ0, si quieres)
        pio_set_irq0_source_enabled(pio0, PIO_INTR_SM0_TXNEMPTY, false); // Deshabilitado si no quieres esta fuente
        // Importante: habilitar la IRQ por el IRQ 0 lanzado en PIO (instr. irq 0)
        pio_set_irq0_source_enabled(pio0, PIO_INTR_IRQ0, true);  // Habilita IRQ0 que lanza la instrucción irq 0
                // Configura el handler IRQ para PIO0_IRQ_0
        flag++;
    }
    printf("Setup IO SM: %d\n ", stepper->stm_sm);
    stepper_1_program_init(stepper->stm_pio, stepper->stm_sm, stm_offset, stepper->STEP_PIN, 1000, true);
    // Por cada SM que uses
    pio_set_irq0_source_enabled(pio0, (enum pio_interrupt_source)(PIO_INTR_SM0_TXNEMPTY + stepper->stm_sm), false);
    pio_set_irq0_source_enabled(pio0, (enum pio_interrupt_source)(PIO_INTR_IRQ0 + stepper->stm_sm), true);
}



void setupGPIO(STEPPER_DRV * stepper)
{

    // stepper enable pin
    gpio_init(stepper->ENABLE_PIN);
    gpio_set_dir(stepper->ENABLE_PIN, GPIO_OUT);

    // stepper STEP pin
    gpio_init(stepper->STEP_PIN);
    gpio_set_dir(stepper->STEP_PIN, GPIO_OUT);

    // stepperMotor1 dir pin
    gpio_init(stepper->DIR_PIN);
    gpio_set_dir(stepper->DIR_PIN, GPIO_OUT);
    // gpio_put(stepperMotor1.DIR_PIN, 1);

}

// Handler de interrupción para pio0
void pio0_irq0_handler() {
    
    // Leer el registro de interrupciones IRQ del PIO0
    uint32_t irq_status = pio0->irq;
    
    // Limpiar la IRQ para poder recibir otra
    pio_interrupt_clear(pio0, 0);
    printf("%b\n", irq_status);

    if (irq_status & (1 << 0)) {
        printf("Interrupción de SM0\n");
    }
    if (irq_status & (1 << 1)) {
        printf("Interrupción de SM1\n");
    }
    if (irq_status & (1 << 2)) {
        printf("Interrupción de SM2\n");
    }
    if (irq_status & (1 << 3)) {
        printf("Interrupción de SM3\n");
    }
   

    pio0->irq = irq_status; // limpiar los bits leídos
    printf("Stepper done!\n");
    // Forzar IRQ1
    //pio_sm_put(pio0, 1, 5000);
    
}


void setFrec( STEPPER_DRV * stepper, float desired_freq){
    float new_div = clock_get_hz(clk_sys) / (desired_freq*CYCLES_PER_STEP);
    pio_sm_set_clkdiv(stepper->stm_pio, stepper->stm_sm , new_div);
}

void setDirection( STEPPER_DRV * stepper,int direction){
    switch (direction)
    {
    case -1:
        stepper->dir = LEFT;
        gpio_put(stepper->DIR_PIN, false);
        break;
    case 1:
        stepper->dir = RIGHT;
        gpio_put(stepper->DIR_PIN, true);
        break;
    default:
     printf("DIRECTION ERROR\n");
        break;
    }
}

void moveSteps(STEPPER_DRV * stepper, int steps ){
    if(steps == 0){
        printf("STEPS MUST BE DIFFERENT THAN ZERO\n");
        return;
    }else if(steps > 0){
        setDirection(stepper, RIGHT);
    }else{
        setDirection(stepper, LEFT);
    }
    gpio_put(stepper->ENABLE_PIN, true);
    pio_sm_put_blocking(stepper->stm_pio, stepper->stm_sm , abs(steps));
}


void moveStepsAtSpeed(STEPPER_DRV * stepper, int steps, float frec){
    setFrec(stepper, frec);
    moveSteps(stepper, steps);
}
