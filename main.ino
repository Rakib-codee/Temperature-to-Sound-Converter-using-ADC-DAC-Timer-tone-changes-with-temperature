/**
 * @file main.ino
 * @brief Temperature-to-Sound Converter using STM32
 * @description Reads temperature via ADC and converts it to sound frequency via DAC + Timer
 * 
 * Hardware Configuration:
 * - ADC1 Channel 0 (PA0): Temperature sensor input (potentiometer for simulation)
 * - DAC1 Channel 1 (PA5): Audio output to speaker
 * - TIM2: Timer for tone generation
 * 
 * Simulation Setup:
 * - Connect potentiometer to PA0 (simulates temperature sensor)
 * - Connect speaker to PA5 via 1kΩ resistor
 * - Adjust potentiometer to change temperature -> frequency changes
 */

#include <Arduino.h>

// Pin definitions
#define TEMP_SENSOR_PIN PA0    // ADC input (potentiometer in simulation)
#define AUDIO_OUT_PIN PA5       // DAC output (to speaker)

// Frequency range
#define MIN_FREQ 200   // Minimum frequency (Hz) - Low temperature
#define MAX_FREQ 2000  // Maximum frequency (Hz) - High temperature

// Global variables
uint32_t current_frequency = 440;  // Current frequency in Hz
uint32_t last_update = 0;
const uint32_t UPDATE_INTERVAL = 100; // Update frequency every 100ms

/**
 * @brief Setup function - runs once at startup
 */
void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("STM32 Temperature-to-Sound Converter");
    Serial.println("=====================================");
    Serial.println("Adjust potentiometer to change temperature");
    Serial.println("Frequency range: 200-2000 Hz");
    Serial.println();
    
    // Configure ADC pin
    pinMode(TEMP_SENSOR_PIN, INPUT_ANALOG);
    
    // Configure DAC pin (PA5)
    pinMode(AUDIO_OUT_PIN, OUTPUT);
    
    // Initialize DAC
    dac_init();
    
    // Initialize timer for tone generation
    timer_init();
    
    // Start with initial frequency
    set_frequency(440);
}

/**
 * @brief Main loop - runs continuously
 */
void loop() {
    // Read temperature sensor (ADC value 0-4095)
    uint16_t adc_value = analogRead(TEMP_SENSOR_PIN);
    
    // Convert ADC value to frequency
    uint32_t new_frequency = map_frequency(adc_value);
    
    // Update frequency if changed significantly or time interval passed
    if (abs((int32_t)new_frequency - (int32_t)current_frequency) > 5 || 
        (millis() - last_update) > UPDATE_INTERVAL) {
        
        current_frequency = new_frequency;
        set_frequency(current_frequency);
        last_update = millis();
        
        // Debug output
        Serial.print("ADC: ");
        Serial.print(adc_value);
        Serial.print(" | Frequency: ");
        Serial.print(current_frequency);
        Serial.println(" Hz");
    }
    
    // Generate tone using DAC
    generate_tone();
    
    delay(10); // Small delay
}

/**
 * @brief Map ADC value to frequency
 * @param adc_value: ADC reading (0-4095)
 * @return Frequency in Hz (200-2000 Hz range)
 */
uint32_t map_frequency(uint16_t adc_value) {
    // Linear mapping: ADC 0-4095 -> Frequency 200-2000 Hz
    uint32_t frequency = MIN_FREQ + ((uint32_t)adc_value * (MAX_FREQ - MIN_FREQ)) / 4095;
    
    // Clamp to valid range
    if (frequency < MIN_FREQ) frequency = MIN_FREQ;
    if (frequency > MAX_FREQ) frequency = MAX_FREQ;
    
    return frequency;
}

/**
 * @brief Set output frequency
 * @param frequency: Desired frequency in Hz
 */
void set_frequency(uint32_t frequency) {
    // The frequency is controlled by the sine wave generation in generate_tone()
    // The actual PWM carrier frequency is set by the STM32 Arduino core
    // We control the audio frequency by changing the phase increment rate
    // This function is called when frequency changes to update the system
    current_frequency = frequency;
}

/**
 * @brief Generate tone using DAC
 */
void generate_tone() {
    // Generate sine wave using phase accumulator
    static double phase = 0.0;
    static uint32_t last_time = 0;
    uint32_t current_time = micros();
    uint32_t elapsed = current_time - last_time;
    
    if (elapsed > 0 && current_frequency > 0) {
        // Calculate phase increment: 2π * frequency * time_elapsed
        // elapsed is in microseconds, convert to seconds
        double time_seconds = (double)elapsed / 1000000.0;
        double phase_increment = 2.0 * PI * current_frequency * time_seconds;
        
        // Update phase
        phase += phase_increment;
        
        // Keep phase in 0-2π range
        while (phase >= 2.0 * PI) {
            phase -= 2.0 * PI;
        }
        
        last_time = current_time;
        
        // Generate sine wave value (0-4095 for 12-bit DAC)
        // Center at 2048 (midpoint) with amplitude of 2047
        int16_t sine_value = 2048 + (int16_t)(2047 * sin(phase));
        
        // Clamp value to valid range
        if (sine_value < 0) sine_value = 0;
        if (sine_value > 4095) sine_value = 4095;
        
        // Write to DAC using analogWrite (PWM mode for simulation)
        // For STM32, PA5 supports PWM on TIM2_CH1
        // Scale 12-bit value (0-4095) to 8-bit (0-255) for analogWrite
        analogWrite(AUDIO_OUT_PIN, sine_value >> 4);
    } else if (elapsed == 0) {
        last_time = current_time;
    }
}

/**
 * @brief Initialize DAC
 */
void dac_init() {
    // For STM32F103C8, we use PWM on PA5 instead of true DAC
    // PA5 is TIM2_CH1, which we'll configure for PWM
    // In Wokwi simulation, analogWrite() handles this automatically
    pinMode(AUDIO_OUT_PIN, OUTPUT);
}

/**
 * @brief Initialize timer
 */
void timer_init() {
    // Timer is used implicitly by analogWrite() for PWM generation
    // For STM32F103C8, PA5 uses TIM2_CH1
    // The frequency is controlled by the PWM frequency set by analogWriteFrequency()
    // Note: STM32 Arduino core handles timer configuration automatically
}

