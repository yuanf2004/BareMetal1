#include "led_controller.h"

/* Red is Pin A1 and Green is Pin A5 */

/* 0 for red and 1 for green */
int active_led = 0;
/* Track how many ticks have occurred */
volatile uint32_t ms_ticks = 0;

void init(){
    /* Enable interrupts through CMSIS *Cortex Microcontroller Software Interface Standard */
    __enable_irq();
    init_systick();

    /* Enable peripheral clock for GPIOA and GPIOB */
    RCC_AHB1ENR &= ~(0x03);
    RCC_AHB1ENR |= 0x3;

    /* Init LED pins */
    init_leds();

    /* Init button pin */
    init_button();
}

/*
Sources:
https://developer.arm.com/documentation/dui0553/latest
https://developer.arm.com/documentation/100166/0001/System-Control/System-control-registers?lang=en
*/
void init_systick(){
    /* Match the 16 MHz clock cycle but minus 1 since it starts at 0 */
    SYSTICK_RLD = 15999; 
    SYSTICK_CUR_VAL = 0;
    /* Clear bits [2:0] */
    SYSTICK_CTRL &= ~(0x7);
    /* 
    CLKSOURCE = 1 = Processor Clock
    TICKINT = 1 = SysTick Exception Request on 0 for interrupt
    ENABLE = 1 
    */
    SYSTICK_CTRL |= 0x7;
}   

void SysTick_Handler(void){
    ms_ticks++;
}

void init_leds(){
/* Reset pins A1 and A5 (bits [5:2]) */
GPIOA_MODER &= ~((0x3 << (1 * 2)) | (0x3 << (5 * 2)));

/* Set pins A1 and A5 to output */
GPIOA_MODER |= ((0x1 << (1 * 2)) | (0x1 << (5 * 2)));

/* Start with red (0) on and green (1) off */
clear_pins();
GPIOA_ODR |= (0x1 << 1); 
} 

void init_button(){
/* Clear pin B1 to 00, which is also input */
GPIOB_MODER &= ~(0x03 << (1 * 2));

/* Set pin B1 to pull-down */ 
GPIOB_PUPDR |= (0x02 << (1 * 2));
}

/* Wrapper function to run */
void run(){
    static uint32_t last_toggle = 0;
    uint32_t button_state = read_button();
    if(button_state && ms_ticks - last_toggle > 200){
        clear_pins();
        handle_leds();
        active_led = !active_led;
        last_toggle = ms_ticks;
    }
}


/* Return 1 if reading high and 0 if reading low */
uint32_t read_button(){
    /* If the button is being pressed (1) */
    if((GPIOB_IDR & (0x01 << 1)) == (0x01 << 1)){   
        return 1;
    }
    else{
        return 0;
    }
}

/* Clear both LED pins */
void clear_pins(){
    GPIOA_ODR &= ~((0x1 << 1) | (0x1 << 5));
}

/* Set the red LED high - Pin A1 (0) */
void set_red_high(){
    GPIOA_ODR |= (0x1 << 1);
}

/* Set the Green LED high - Pin A2 (1) */
void set_green_high(){
    GPIOA_ODR |= (0x1 << 5);
}

/* Handle the toggling of LEDs*/
void handle_leds(){
    /* Set red if green is high */
    if(active_led == 1){
        set_red_high();
    }
    /* Set green if red is high */
    else if(active_led == 0){
        set_green_high();
    }
}
