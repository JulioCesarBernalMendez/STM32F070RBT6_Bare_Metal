#define GPIOA_EN_BIT        (0x01U << 17)
#define GPIOC_EN_BIT        (0x01U << 19)

#define GPIOx_PIN_5         (0x01U << 5)
#define GPIOx_PIN_13        (0x01U << 13)
#define LED_PIN             (GPIOx_PIN_5)
#define BUTTON_PIN          (GPIOx_PIN_13)

#define GPIOA_CLK_ENBL()    (RCC->AHBENR |= GPIOA_EN_BIT)
#define GPIOC_CLK_ENBL()    (RCC->AHBENR |= GPIOC_EN_BIT)
