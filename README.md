# STM32 Temperature-to-Sound Converter

## Project Overview

This project implements a **Temperature-to-Sound Converter** using STM32 microcontroller. The system reads temperature values via ADC and converts them into audio frequencies using DAC and Timer peripherals. As the temperature changes, the output tone frequency changes accordingly.

## Features

- **ADC (Analog-to-Digital Converter)**: Reads temperature sensor input
- **DAC (Digital-to-Analog Converter)**: Outputs analog audio signal
- **Timer (TIM2)**: Generates precise frequency tones
- **Real-time Conversion**: Temperature changes are immediately reflected in sound frequency
- **Frequency Range**: 200 Hz to 2000 Hz (adjustable)
- **Web Simulation**: Fully functional browser-based simulation

## Web Simulation

### Quick Start

1. **Open the simulation**:
   - Simply open `index.html` in any modern web browser
   - No installation or setup required!

2. **Use the simulation**:
   - Adjust the temperature slider to change the ADC input (0-4095)
   - Click "Start Audio" to hear the sound output
   - Watch real-time updates of ADC values, temperature, and frequency
   - See the waveform visualization

### Web Simulation Features

- ✅ **Interactive Temperature Control**: Slider simulates ADC input (0-4095)
- ✅ **Real-time Audio Output**: Web Audio API generates sine wave tones
- ✅ **Visual Feedback**: 
  - Live waveform display
  - Frequency indicator bar
  - Musical note display
- ✅ **Pin Status Display**: Shows PA0 (ADC) and PA5 (DAC) values
- ✅ **No External Dependencies**: Works offline, no internet required

### How to Run

**Option 1: Direct File Open**
- Double-click `index.html` to open in your default browser

**Option 2: Local Server (Recommended)**
```bash
# Using Python 3
python -m http.server 8000

# Using Python 2
python -m SimpleHTTPServer 8000

# Using Node.js (if you have http-server installed)
npx http-server
```
Then open `http://localhost:8000` in your browser.

## Hardware Configuration (For Physical Implementation)

### Components Required:
- STM32F103C8 (Blue Pill) or compatible STM32 board
- Temperature sensor (e.g., LM35, DS18B20, or potentiometer for testing)
- Speaker or buzzer
- 1kΩ resistor (for speaker protection)
- Connecting wires

### Pin Connections:
- **PA0 (ADC1_IN0)**: Temperature sensor input
- **PA5 (DAC1_OUT1)**: Audio output to speaker
- **GND**: Common ground
- **3.3V**: Power supply

## How It Works

1. **Temperature Reading**: ADC continuously reads the analog voltage from the temperature sensor (0-3.3V, mapped to 0-4095 digital value)

2. **Frequency Mapping**: The ADC value is linearly mapped to a frequency range:
   - ADC = 0 → 200 Hz (low temperature)
   - ADC = 4095 → 2000 Hz (high temperature)
   - Formula: `Frequency = 200 + (ADC_value × 1800 / 4095)`

3. **Tone Generation**: 
   - Timer (TIM2) generates the base frequency
   - DAC converts digital values to analog sine wave
   - Output is sent to speaker via PA5

4. **Real-time Updates**: System updates frequency every 100ms or when significant change is detected

## Code Files

### For Web Simulation:
- **index.html**: Complete web simulation with HTML, CSS, and JavaScript
  - Interactive UI with slider control
  - Real-time audio generation using Web Audio API
  - Visual waveform display
  - All STM32 functionality simulated

### For STM32 Hardware:
- **main.ino**: Arduino-compatible code for STM32
  - Uses Arduino framework for easier development
  - Compatible with STM32duino/Arduino_STM32
  - Ready for physical hardware implementation

- **main.c**: Low-level STM32 HAL code
  - Direct register manipulation
  - For STM32F4 series (can be adapted for F1)
  - Note: Requires STM32 HAL libraries for compilation
  - The IDE may show errors because headers aren't in this environment, but the code is correct for embedded use

## Code Explanation

### Main Components:

1. **ADC Reading** (`analogRead(PA0)` or `ADC_Read_Temperature()`):
   - Reads 12-bit ADC value (0-4095)
   - Represents temperature sensor voltage

2. **Frequency Calculation** (`map_frequency()` or `Temperature_To_Frequency()`):
   - Maps ADC value to frequency range
   - Linear interpolation between MIN_FREQ and MAX_FREQ

3. **Tone Generation** (`generate_tone()` or Timer/DAC configuration):
   - Uses phase accumulator for sine wave generation
   - Updates DAC output continuously
   - Frequency controlled by phase increment

4. **Timer Configuration** (`set_frequency()` or `TIM2_Init()`):
   - Configures TIM2 for desired frequency
   - Used for precise timing control

## Customization

### Adjust Frequency Range:
Modify these constants in `main.ino`:
```cpp
#define MIN_FREQ 200   // Minimum frequency (Hz)
#define MAX_FREQ 2000  // Maximum frequency (Hz)
```

Or in `index.html` JavaScript:
```javascript
const frequency = 200 + Math.round((adcValue * 1800) / 4095);
// Change 200 and 1800 to adjust range
```

### Change Update Rate:
Modify `UPDATE_INTERVAL`:
```cpp
const uint32_t UPDATE_INTERVAL = 100; // milliseconds
```

### Use Different Pins:
Change pin definitions:
```cpp
#define TEMP_SENSOR_PIN PA0  // Change to desired ADC pin
#define AUDIO_OUT_PIN PA5    // Change to desired DAC pin
```

## Testing

### In Web Simulation:
1. Open `index.html` in browser
2. Adjust slider from 0% to 100%
3. Click "Start Audio"
4. Observe:
   - Real-time ADC values and frequencies
   - Audio output changes with slider
   - Waveform visualization
   - Frequency increases linearly with slider position

### Expected Behavior:
- **Slider at 0%**: ~200 Hz (low pitch)
- **Slider at 50%**: ~1100 Hz (medium pitch)
- **Slider at 100%**: ~2000 Hz (high pitch)

### On Physical Hardware:
1. Connect temperature sensor to PA0
2. Connect speaker to PA5 via resistor
3. Upload `main.ino` to STM32
4. Monitor Serial output (115200 baud)
5. Temperature changes should affect audio frequency

## Troubleshooting

### Web Simulation Issues:
- **No sound**: Check browser audio permissions, ensure audio is not muted
- **Slider not working**: Check browser console for JavaScript errors
- **Waveform not displaying**: Ensure canvas element is properly loaded

### Hardware Issues:
- **No sound output**: Check speaker connections, verify DAC pin (PA5)
- **Frequency not changing**: Verify ADC reading is working (check Serial output)
- **Compilation errors**: Ensure STM32 board package is installed in Arduino IDE

## Project Requirements Met

✅ **ADC**: Reads temperature sensor input  
✅ **DAC**: Outputs analog audio signal  
✅ **Timer**: Generates precise frequency tones  
✅ **Temperature-to-Sound**: Converts temperature to frequency  
✅ **Web Simulation**: Fully functional browser-based simulation  

## Project Structure

```
EMBEDDED proect/
├── index.html          # Web simulation (HTML/CSS/JavaScript)
├── main.ino            # Arduino code for STM32 hardware
├── main.c              # Low-level STM32 HAL code
├── README.md           # This file
└── PROJECT_SUMMARY.md  # Technical project summary
```

## Future Enhancements

- Add multiple temperature ranges with different frequency mappings
- Implement non-linear frequency mapping (logarithmic scale)
- Add visual feedback (LED indicators) in web simulation
- Support for multiple audio channels
- Temperature calibration and offset adjustment
- Export audio recording feature

## Browser Compatibility

The web simulation works best in:
- ✅ Chrome/Edge (recommended)
- ✅ Firefox
- ✅ Safari
- ✅ Opera

**Note**: Some older browsers may not support Web Audio API.

## Author

Created for Embedded Systems course final project.

## License

Educational use only.

---

**Note**: 
- The web simulation (`index.html`) works immediately without any setup
- For physical hardware, use `main.ino` with Arduino IDE and STM32 board support
- The `main.c` file shows low-level implementation but requires STM32 HAL libraries to compile
