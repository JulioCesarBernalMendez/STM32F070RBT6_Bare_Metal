#include "gpio.h"

void GPIO_Interrupt_Init( void )
{   
    /* Disable global interrupts */
    __disable_irq();

    /* Initialize and configure GPIOC peripheral */
    GPIOC_Init();

    /* Enable global interrupts */
    __enable_irq();
}

void GPIOC_Init( void )
{
    /* Enable GPIOC clock access */
    GPIOC_CLK_ENBL();

    /* GPIOC0 (INT1 of ADXL345) as input */
    GPIOC->MODER &= ~GPIO_MODER_MODER0_1;
    GPIOC->MODER &= ~GPIO_MODER_MODER0_0;

    /* GPIOC0 (INT1) with pull-down resistor */
    GPIOC->PUPDR |=  GPIO_PUPDR_PUPDR0_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR0_0;

    /* GPIOC1 (INT2 of ADXL345) as input */
    GPIOC->MODER &= ~GPIO_MODER_MODER1_1;
    GPIOC->MODER &= ~GPIO_MODER_MODER1_0;

    /* GPIOC1 (INT2) with pull-down resistor */
    GPIOC->PUPDR |=  GPIO_PUPDR_PUPDR1_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR1_0;

    /* Configure GPIOC0 (INT1) and GPIOC1 (INT2) as
       interrupts on the rising edge */
    GPIOC_EXTI0_1_Init();

    /* GPIOC13 (board button) as input */
    GPIOC->MODER &= ~GPIO_MODER_MODER13_1;
    GPIOC->MODER &= ~GPIO_MODER_MODER13_0;

    /* GPIOC13 without pull-up/pull-down (it already uses a pull-up resistor) */
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_0;

    /* Configure GPIOC13 (Board button) as interrupt on the falling edge  */
    GPIOC_EXTI13_Init();
}

void GPIOC_EXTI0_1_Init( void )
{
    /* Enable SYSCFG clock access */
    SYSCFG_CLK_ENBL();
    
    /* Map GPIOC0 (INT1 of ADXL345) to EXTI0 line for interrupt */
    SYSCFG->EXTICR[ 0 ] |= SYSCFG_EXTICR1_EXTI0_PC;

    /* Unmask/enable interrupt on EXTI line 0 (GPIOC0) */
    EXTI->IMR |= EXTI_IMR_IM0;

    /* EXTI0 (GPIOC0) interrupt triggered on the rising edge */
    EXTI->RTSR |= EXTI_RTSR_RT0;

    /* EXTI0 (GPIOC0) interrupt falling trigger disabled */
    EXTI->FTSR &= ~EXTI_FTSR_FT0;

    /* Map GPIOC1 (INT2 of ADXL345) to EXTI1 line for interrupt */
    SYSCFG->EXTICR[ 0 ] |= SYSCFG_EXTICR1_EXTI1_PC;

    /* Unmask/enable interrupt on line 1 (GPIOC1) */
    EXTI->IMR |= EXTI_IMR_IM1;

    /* EXTI1 (GPIOC1) interrupt triggered on the rising edge */
    EXTI->RTSR |= EXTI_RTSR_RT1;

    /* EXTI1 (GPIOC1) interrupt falling trigger disabled */
    EXTI->FTSR &= ~EXTI_FTSR_FT1;

    /* Enable interrupt 5 (EXTI0_1) of the NVIC */
    NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_5;

    /* Set interrupt EXTI0_1 (N=5) priority to zero (highest priority).
       IPRx. With x = N / 4 = 5 / 4, then x = 1. Therefore IPRx = IPR1.
       Byte offset = N MOD 4 = 5 MOD 4, therefore Byte offset = 1.
       NOTE: processor implements only bits 7 and 6 of each field. */
    NVIC->IP[ 1 ] &= ~NVIC_IPR1_PRI5_7;
    NVIC->IP[ 1 ] &= ~NVIC_IPR1_PRI5_6;
}

void GPIOC_EXTI13_Init( void )
{
    /* Enable SYSCFG clock access */
    SYSCFG_CLK_ENBL();

    /* Map GPIOC13 (board button) to the EXTI13 line */
    SYSCFG->EXTICR[ 3 ] |= SYSCFG_EXTICR4_EXTI13_PC;

    /* Unmask/enable interrupt on EXTI line 13 (GPIOC13) */
    EXTI->IMR |= EXTI_IMR_IM13;

    /* EXTI13 (GPIOC13) interrupt triggered on the falling edge
       NOTE: board button uses a pull-up resistor, its state when pressed is LOW */
    EXTI->FTSR |= EXTI_FTSR_FT13;

    /* EXTI13 (GPIOC13) interrupt rising trigger disabled */
    EXTI->RTSR &= ~EXTI_RTSR_RT13;

    /* Enable interrupt 7 (EXTI4_15) of the NVIC */
    NVIC->ISER[ 0 ] |= NVIC_ISER_SETENA_7;

    /* Set interrupt EXTI4_15 (N=7) priority to 64 (second highest).
       IPRx. With x = N / 4 = 7 / 4, then x = 1. Therefore IPRx = IPR1.
       Byte offset = N MOD 4 = 7 MOD 4, therefore Byte offset = 3.
       NOTE: processor implements only bits 7 and 6 of each field. */
    NVIC->IP[ 1 ] &= ~NVIC_IPR1_PRI7_7;
    NVIC->IP[ 1 ] |=  NVIC_IPR1_PRI7_6;
}
