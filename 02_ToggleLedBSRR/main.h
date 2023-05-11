#define GPIOA_EN_BIT        (0x01U << 17)

#define GPIOx_PIN_5         (0x01U << 5)
#define LED_PIN             (GPIOx_PIN_5)

#define GPIOA_CLK_ENBL()    (RCC->AHBENR |= GPIOA_EN_BIT)
