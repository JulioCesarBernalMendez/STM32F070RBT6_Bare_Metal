#define PERIPH_BASE            (0x40000000UL)
 
#define AHB1_PERIPH_OFFSET     (0x00020000UL)
#define AHB1_PERIPH_BASE       (PERIPH_BASE + AHB1_PERIPH_OFFSET)

#define RCC_PERIPH_OFFSET      (0x00001000UL)
#define RCC_PERIPH_BASE        (AHB1_PERIPH_BASE + RCC_PERIPH_OFFSET)

#define GPIOA_PERIPH_OFFSET    (0x08000000UL)
#define GPIOA_PERIPH_BASE      (PERIPH_BASE + GPIOA_PERIPH_OFFSET)

#define GPIOA_EN_BIT           (0x01U << 17)

#define GPIOx_PIN_5            (0x01U << 5)
#define LED_PIN                GPIOx_PIN_5

#define __IO                   volatile

typedef struct
{
    __IO uint32_t CR;          /* Clock control register,                 offset 0x00 */
    __IO uint32_t CFGR;        /* Clock configuration register,           offset 0x04 */
    __IO uint32_t CIR;         /* Clock interrupt register,               offset 0x08 */
    __IO uint32_t APB2RSTR;    /* APB peripheral reset register 2,        offset 0x0C */
    __IO uint32_t APB1RSTR;    /* APB peripheral reset register 1,        offset 0x10 */
    __IO uint32_t AHBENR;      /* AHB peripheral clock enable register,   offset 0x14 */
    __IO uint32_t APB2ENR;     /* APB peripheral clock enable register 2, offset 0x18 */
    __IO uint32_t APB1ENR;     /* APB peripheral clock enable register 1, offset 0x1C */
    __IO uint32_t BDCR;        /* RTC domain control register,            offset 0x20 */
    __IO uint32_t CSR;         /* Control/Status register,                offset 0x24 */
    __IO uint32_t AHBRSTR;     /* AHB peripheral reset register,          offset 0x28 */
    __IO uint32_t CFGR2;       /* Clock configuration register 2,         offset 0x2C */
    __IO uint32_t CFGR3;       /* Clock configuration register 3,         offset 0x30 */
    __IO uint32_t CR2;         /* Clock control register 2,               offset 0x34 */
} RCC_TypeDef;

typedef struct
{
    __IO uint32_t MODER;       /* GPIO port mode register,               offset 0x00 */
    __IO uint32_t OTYPER;      /* GPIO port output type register,        offset 0x04 */
    __IO uint32_t OSPEEDR;     /* GPIO port output speed register,       offset 0x08 */
    __IO uint32_t PUPDR;       /* GPIO port pull-up/pull-down register,  offset 0x0C */
    __IO uint32_t IDR;         /* GPIO port input data register,         offset 0x10 */
    __IO uint32_t ODR;         /* GPIO port output data register,        offset 0x14 */
    __IO uint32_t BSRR;        /* GPIO port bit set/reset register,      offset 0x18 */
    __IO uint32_t LCKR;        /* GPIO port configuration lock register, offset 0x1C */
    __IO uint32_t AFRL;        /* GPIO alternate function low register,  offset 0x20 */
    __IO uint32_t AFRH;        /* GPIO alternate function high register, offset 0x24 */
    __IO uint32_t BRR;         /* GPIO port bit reset register,          offset 0x28 */
} GPIO_TypeDef;

#define RCC                 ((RCC_TypeDef*)(RCC_PERIPH_BASE))
#define GPIOA               ((GPIO_TypeDef*)(GPIOA_PERIPH_BASE))

#define GPIOA_CLK_ENBL()    ( RCC->AHBENR |= GPIOA_EN_BIT )