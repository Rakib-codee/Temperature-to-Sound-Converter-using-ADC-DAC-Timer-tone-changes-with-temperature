/**
 * @file main.c
 * @brief Temperature-to-Sound Converter using STM32
 * @description Reads temperature via ADC and converts it to sound frequency via DAC + Timer
 * 
 * Hardware Configuration:
 * - ADC1 Channel 0 (PA0): Temperature sensor input (simulated)
 * - DAC1 Channel 1 (PA5): Audio output
 * - TIM2: Timer for tone generation
 * 
 * NOTE: A mock header file (stm32f4xx.h) is provided for code validation.
 * For actual STM32 development, use the official STM32 HAL libraries.
 * The code compiles successfully with the mock header for syntax checking.
 * 
 * For web simulation, use index.html instead.
 * For Arduino-compatible code, use main.ino instead.
 */

#include "stm32f4xx.h"
#include <math.h>
#include <stdlib.h>  // For abs() function

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void ADC1_Init(void);
void DAC1_Init(void);
void TIM2_Init(uint32_t frequency);
uint16_t ADC_Read_Temperature(void);
uint32_t Temperature_To_Frequency(uint16_t adc_value);
void Delay_ms(uint32_t ms);

// Global variables
volatile uint32_t current_frequency = 440; // Default frequency (A4 note)

/**
 * @brief Main function
 */
int main(void)
{
    // System initialization
    SystemClock_Config();
    GPIO_Init();
    ADC1_Init();
    DAC1_Init();
    TIM2_Init(440); // Start with 440 Hz (A4 note)
    
    // Enable interrupts
    __enable_irq();
    
    // Main loop
    while (1)
    {
        // Read temperature from ADC
        uint16_t adc_value = ADC_Read_Temperature();
        
        // Convert temperature to frequency
        uint32_t new_frequency = Temperature_To_Frequency(adc_value);
        
        // Update frequency if changed significantly (avoid constant updates)
        if (abs((int32_t)new_frequency - (int32_t)current_frequency) > 5)
        {
            current_frequency = new_frequency;
            TIM2_Init(current_frequency); // Update timer frequency
        }
        
        // Small delay to prevent excessive updates
        Delay_ms(100);
    }
}

/**
 * @brief System Clock Configuration (84 MHz for STM32F4)
 */
void SystemClock_Config(void)
{
    // Enable HSE (High Speed External oscillator)
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    // Configure PLL: HSE (8MHz) * 336 / (8 * 2) = 84 MHz
    RCC->PLLCFGR = (8 << 0) | (336 << 6) | (0 << 16) | (1 << 22) | RCC_PLLCFGR_PLLSRC_HSE;
    
    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    // Configure flash latency
    FLASH->ACR = FLASH_ACR_LATENCY_2WS;
    
    // Select PLL as system clock
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    // Configure AHB, APB1, APB2 prescalers
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;   // AHB = 84 MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  // APB1 = 42 MHz
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;  // APB2 = 84 MHz
}

/**
 * @brief GPIO Initialization
 */
void GPIO_Init(void)
{
    // Enable GPIO clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    // PA0: ADC1_IN0 (Analog mode for temperature sensor)
    GPIOA->MODER |= GPIO_MODER_MODER0; // Analog mode
    
    // PA5: DAC1_OUT1 (Analog mode for audio output)
    GPIOA->MODER |= GPIO_MODER_MODER5; // Analog mode
}

/**
 * @brief ADC1 Initialization (Channel 0 - PA0)
 */
void ADC1_Init(void)
{
    // Enable ADC1 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    
    // Configure ADC1
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC
    
    // Set resolution to 12-bit
    ADC1->CR1 &= ~ADC_CR1_RES;
    
    // Set sample time for channel 0 (144 cycles)
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0;
    
    // Set channel 0 as first in sequence
    ADC1->SQR3 = 0; // Channel 0
    
    // Enable continuous conversion
    ADC1->CR2 |= ADC_CR2_CONT;
    
    // Start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/**
 * @brief Read temperature from ADC
 * @return ADC value (0-4095)
 */
uint16_t ADC_Read_Temperature(void)
{
    // Wait for conversion complete
    while (!(ADC1->SR & ADC_SR_EOC));
    
    // Read ADC value
    return ADC1->DR;
}

/**
 * @brief Convert ADC value to frequency (Hz)
 * @param adc_value: ADC reading (0-4095)
 * @return Frequency in Hz (200-2000 Hz range)
 * 
 * Mapping: ADC 0-4095 -> Frequency 200-2000 Hz
 * This simulates temperature range affecting sound pitch
 */
uint32_t Temperature_To_Frequency(uint16_t adc_value)
{
    // Map ADC value (0-4095) to frequency range (200-2000 Hz)
    // Linear mapping: freq = 200 + (adc_value * 1800 / 4095)
    uint32_t frequency = 200 + ((uint32_t)adc_value * 1800) / 4095;
    
    // Ensure frequency is within valid range
    if (frequency < 200) frequency = 200;
    if (frequency > 2000) frequency = 2000;
    
    return frequency;
}

/**
 * @brief DAC1 Initialization (Channel 1 - PA5)
 */
void DAC1_Init(void)
{
    // Enable DAC clock
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    
    // Enable DAC channel 1
    DAC->CR |= DAC_CR_EN1;
    
    // Enable trigger for DAC channel 1 (TIM2 TRGO)
    DAC->CR |= DAC_CR_TEN1;
    
    // Select TIM2 TRGO as trigger source
    DAC->CR |= (2 << DAC_CR_TSEL1_Pos);
    
    // Set wave generation mode: Triangle wave
    DAC->CR |= DAC_CR_WAVE1_1; // Triangle wave generation
    DAC->CR |= (0 << DAC_CR_MAMP1_Pos); // Amplitude = 1 LSB
}

/**
 * @brief TIM2 Initialization for tone generation
 * @param frequency: Desired frequency in Hz
 */
void TIM2_Init(uint32_t frequency)
{
    // Disable TIM2 first
    TIM2->CR1 &= ~TIM_CR1_CEN;
    
    // Enable TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    // Calculate prescaler and period for desired frequency
    // System clock = 84 MHz, APB1 = 42 MHz, Timer clock = 84 MHz (if APB1 prescaler != 1)
    uint32_t timer_clock = 84000000; // 84 MHz
    uint32_t period = (timer_clock / frequency) - 1;
    
    // Configure prescaler (1:1)
    TIM2->PSC = 0;
    
    // Configure auto-reload register
    TIM2->ARR = period;
    
    // Enable update event
    TIM2->EGR |= TIM_EGR_UG;
    
    // Configure master mode: Update event as TRGO
    TIM2->CR2 |= TIM_CR2_MMS_1; // Update event as TRGO
    
    // Enable counter
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief Simple delay function
 * @param ms: Delay in milliseconds
 */
void Delay_ms(uint32_t ms)
{
    // Approximate delay (84 MHz system clock)
    for (volatile uint32_t i = 0; i < ms * 8400; i++);
}

/**
 * @brief System Error Handler
 */
void Error_Handler(void)
{
    while (1);
}

