#ifndef SI5351_CONFIG_H
#define SI5351_CONFIG_H

#include <Adafruit_SI5351.h>

// Configuraciones del SI5351
#define SI5351_CRYSTAL_FREQ 25000000  // 25 MHz cristal por defecto

// Rangos de frecuencia válidos
#define MIN_FREQ_HZ     8000UL        // 8 kHz mínimo
#define MAX_FREQ_HZ     160000000UL   // 160 MHz máximo

// Frecuencias por defecto para cada reloj
#define DEFAULT_CLK0_FREQ  1000000UL  // 1 MHz
#define DEFAULT_CLK1_FREQ  2000000UL  // 2 MHz
#define DEFAULT_CLK2_FREQ  3000000UL  // 3 MHz

// Configuraciones I2C
#define I2C_TIMEOUT_MS 1000

// Configuraciones del puerto serie
#define SERIAL_BAUD_RATE 115200
#define COMMAND_BUFFER_SIZE 32

// Estados del LED de estado
#define LED_BLINK_INTERVAL_MS 1000
#define LED_ERROR_BLINK_MS 250

// Validación de frecuencias
inline bool isValidFrequency(unsigned long freq) {
    return (freq >= MIN_FREQ_HZ && freq <= MAX_FREQ_HZ);
}

#endif // SI5351_CONFIG_H
