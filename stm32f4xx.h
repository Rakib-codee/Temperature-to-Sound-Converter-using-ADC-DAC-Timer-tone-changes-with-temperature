/**
 * @file stm32f4xx.h
 * @brief Mock STM32F4xx header file for code validation
 * 
 * NOTE: This is a minimal mock header for syntax checking.
 * For actual STM32 development, use the official STM32 HAL libraries.
 * This file provides basic definitions to allow the code to compile
 * without the full STM32 development environment.
 */

#ifndef STM32F4XX_H
#define STM32F4XX_H

#include <stdint.h>

// Type definitions
typedef uint32_t volatile * const reg32_t;

// RCC (Reset and Clock Control) Register Structure
typedef struct {
    uint32_t CR;        // Clock control register
    uint32_t PLLCFGR;   // PLL configuration register
    uint32_t CFGR;      // Clock configuration register
    uint32_t AHB1ENR;   // AHB1 peripheral clock enable register
    uint32_t APB1ENR;   // APB1 peripheral clock enable register
    uint32_t APB2ENR;   // APB2 peripheral clock enable register
} RCC_TypeDef;

// GPIO Register Structure
typedef struct {
    uint32_t MODER;     // GPIO mode register
    uint32_t OTYPER;    // GPIO output type register
    uint32_t OSPEEDR;   // GPIO output speed register
    uint32_t PUPDR;     // GPIO pull-up/pull-down register
    uint32_t IDR;       // GPIO input data register
    uint32_t ODR;       // GPIO output data register
    uint32_t BSRR;      // GPIO bit set/reset register
    uint32_t LCKR;      // GPIO configuration lock register
    uint32_t AFR[2];    // GPIO alternate function registers
} GPIO_TypeDef;

// ADC Register Structure
typedef struct {
    uint32_t SR;        // ADC status register
    uint32_t CR1;       // ADC control register 1
    uint32_t CR2;       // ADC control register 2
    uint32_t SMPR1;     // ADC sample time register 1
    uint32_t SMPR2;     // ADC sample time register 2
    uint32_t JOFR[4];   // ADC injected channel data offset registers
    uint32_t HTR;       // ADC watchdog higher threshold register
    uint32_t LTR;       // ADC watchdog lower threshold register
    uint32_t SQR1;      // ADC regular sequence register 1
    uint32_t SQR2;      // ADC regular sequence register 2
    uint32_t SQR3;      // ADC regular sequence register 3
    uint32_t JSQR;      // ADC injected sequence register
    uint32_t JDR[4];    // ADC injected data registers
    uint32_t DR;        // ADC regular data register
} ADC_TypeDef;

// DAC Register Structure
typedef struct {
    uint32_t CR;        // DAC control register
    uint32_t SWTRIGR;   // DAC software trigger register
    uint32_t DHR12R1;   // DAC channel1 12-bit right-aligned data holding register
    uint32_t DHR12L1;   // DAC channel1 12-bit left-aligned data holding register
    uint32_t DHR8R1;    // DAC channel1 8-bit right-aligned data holding register
    uint32_t DHR12R2;   // DAC channel2 12-bit right-aligned data holding register
    uint32_t DHR12L2;   // DAC channel2 12-bit left-aligned data holding register
    uint32_t DHR8R2;    // DAC channel2 8-bit right-aligned data holding register
    uint32_t DHR12RD;   // Dual DAC 12-bit right-aligned data holding register
    uint32_t DHR12LD;   // Dual DAC 12-bit left-aligned data holding register
    uint32_t DHR8RD;    // Dual DAC 8-bit right-aligned data holding register
    uint32_t DOR1;      // DAC channel1 data output register
    uint32_t DOR2;      // DAC channel2 data output register
    uint32_t SR;        // DAC status register
} DAC_TypeDef;

// Timer Register Structure
typedef struct {
    uint32_t CR1;       // TIM control register 1
    uint32_t CR2;       // TIM control register 2
    uint32_t SMCR;      // TIM slave mode control register
    uint32_t DIER;      // TIM DMA/interrupt enable register
    uint32_t SR;        // TIM status register
    uint32_t EGR;       // TIM event generation register
    uint32_t CCMR1;     // TIM capture/compare mode register 1
    uint32_t CCMR2;     // TIM capture/compare mode register 2
    uint32_t CCER;      // TIM capture/compare enable register
    uint32_t CNT;       // TIM counter register
    uint32_t PSC;       // TIM prescaler register
    uint32_t ARR;       // TIM auto-reload register
    uint32_t RCR;       // TIM repetition counter register
    uint32_t CCR1;      // TIM capture/compare register 1
    uint32_t CCR2;      // TIM capture/compare register 2
    uint32_t CCR3;      // TIM capture/compare register 3
    uint32_t CCR4;      // TIM capture/compare register 4
    uint32_t BDTR;      // TIM break and dead-time register
    uint32_t DCR;       // TIM DMA control register
    uint32_t DMAR;      // TIM DMA address for full transfer register
} TIM_TypeDef;

// Flash Register Structure
typedef struct {
    uint32_t ACR;       // Flash access control register
    uint32_t KEYR;      // Flash key register
    uint32_t OPTKEYR;   // Flash option key register
    uint32_t SR;        // Flash status register
    uint32_t CR;        // Flash control register
    uint32_t OPTCR;     // Flash option control register
    uint32_t OPTCR1;    // Flash option control register 1
} FLASH_TypeDef;

// Peripheral Base Addresses
#define PERIPH_BASE           0x40000000UL
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)

// RCC Base Address
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define RCC                    ((RCC_TypeDef *)RCC_BASE)

// GPIO Base Addresses
#define GPIOA_BASE             (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOA                   ((GPIO_TypeDef *)GPIOA_BASE)

// ADC Base Addresses
#define ADC1_BASE              (APB2PERIPH_BASE + 0x2400UL)
#define ADC1                   ((ADC_TypeDef *)ADC1_BASE)

// DAC Base Address
#define DAC_BASE               (APB1PERIPH_BASE + 0x7400UL)
#define DAC                    ((DAC_TypeDef *)DAC_BASE)

// Timer Base Addresses
#define TIM2_BASE              (APB1PERIPH_BASE + 0x0000UL)
#define TIM2                    ((TIM_TypeDef *)TIM2_BASE)

// Flash Base Address
#define FLASH_BASE             0x40023C00UL
#define FLASH                   ((FLASH_TypeDef *)FLASH_BASE)

// RCC Register Bits
#define RCC_CR_HSEON           (1UL << 16)
#define RCC_CR_HSERDY          (1UL << 17)
#define RCC_CR_PLLON           (1UL << 24)
#define RCC_CR_PLLRDY          (1UL << 25)
#define RCC_PLLCFGR_PLLSRC_HSE (1UL << 22)

#define RCC_CFGR_SW_PLL        (2UL << 0)
#define RCC_CFGR_SWS           (3UL << 2)  // System clock switch status mask
#define RCC_CFGR_SWS_PLL       (2UL << 2)
#define RCC_CFGR_HPRE_DIV1     (0UL << 4)
#define RCC_CFGR_PPRE1_DIV2    (4UL << 10)
#define RCC_CFGR_PPRE2_DIV1    (0UL << 13)

#define RCC_AHB1ENR_GPIOAEN    (1UL << 0)
#define RCC_APB1ENR_DACEN      (1UL << 29)
#define RCC_APB1ENR_TIM2EN     (1UL << 0)
#define RCC_APB2ENR_ADC1EN      (1UL << 8)

// GPIO Register Bits
#define GPIO_MODER_MODER0      (3UL << 0)
#define GPIO_MODER_MODER5      (3UL << 10)

// ADC Register Bits
#define ADC_SR_EOC             (1UL << 1)
#define ADC_CR1_RES            (3UL << 24)
#define ADC_CR2_ADON           (1UL << 0)
#define ADC_CR2_CONT           (1UL << 1)
#define ADC_CR2_SWSTART        (1UL << 30)
#define ADC_SMPR2_SMP0_0       (1UL << 0)
#define ADC_SMPR2_SMP0_1       (1UL << 1)
#define ADC_SMPR2_SMP0_2       (1UL << 2)

// DAC Register Bits
#define DAC_CR_EN1             (1UL << 0)
#define DAC_CR_TEN1            (1UL << 2)
#define DAC_CR_TSEL1_Pos       3
#define DAC_CR_WAVE1_1         (1UL << 6)
#define DAC_CR_MAMP1_Pos       8

// Timer Register Bits
#define TIM_CR1_CEN            (1UL << 0)
#define TIM_CR2_MMS_1          (2UL << 4)
#define TIM_EGR_UG             (1UL << 0)

// Flash Register Bits
#define FLASH_ACR_LATENCY_2WS  (2UL << 0)

// Intrinsic Functions
// Note: For actual STM32 compilation, use the official STM32 HAL libraries
// which provide proper ARM-specific implementations
#ifdef STM32_TARGET_COMPILATION
    // Only use ARM assembly when explicitly targeting STM32
    // This requires proper ARM toolchain configuration
    #if defined(__GNUC__) && (defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__CORTEX_M))
        #define __enable_irq()      __asm__ volatile("cpsie i" ::: "memory")
        #define __disable_irq()     __asm__ volatile("cpsid i" ::: "memory")
    #else
        #define __enable_irq()      do {} while(0)
        #define __disable_irq()     do {} while(0)
    #endif
#else
    // Default: no-op implementations for code validation on non-ARM platforms
    // These allow the code to compile for syntax checking
    #define __enable_irq()      do {} while(0)
    #define __disable_irq()     do {} while(0)
#endif

#endif /* STM32F4XX_H */

