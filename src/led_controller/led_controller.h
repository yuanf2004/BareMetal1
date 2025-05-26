#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <stdint.h>
#include "stm32f446xx.h"

/* Activate Systick on the Cortex-M4 Processor */
#define SYSTICK_CTRL *((uint32_t *)(0xE000E010))
#define SYSTICK_RLD *((uint32_t *)(0xE000E014))
#define SYSTICK_CUR_VAL *((uint32_t *)(0xE000E018))

/* GPIOA and GPIOB peripheral clock */
#define RCC_AHB1ENR *((uint32_t *)(0x40023800 + 0x30))

/* GPIOA */
#define GPIOA_MODER *((uint32_t *)(0x40020000))
#define GPIOA_ODR *((uint32_t *)(0x40020000 + 0x14))

/* GPIOB */
#define GPIOB_MODER *((uint32_t *)(0x40020400))
#define GPIOB_PUPDR *((uint32_t *)(0x40020400 + 0x0C))
#define GPIOB_IDR *((uint32_t *)(0x40020400 + 0x10))

extern int active_led;
extern volatile uint32_t ms_ticks;

void init();

void init_systick();

void SysTickHandler(void);

void init_leds();

void init_button(); 

void run();

uint32_t read_button();

void clear_pins();

void set_red_high();

void set_green_high();

void handle_leds();

#endif