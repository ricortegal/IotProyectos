/*
 * Ejemplos de uso avanzado del SI5351
 * ==================================
 * Usando Adafruit Si5351 Library
 */

#ifndef SI5351_EXAMPLES_H
#define SI5351_EXAMPLES_H

#include <Adafruit_SI5351.h>

// Ejemplo 1: Configuración para radioaficionado
void setupHamRadio(Adafruit_SI5351& clockgen) {
    // Configurar PLLs para las frecuencias deseadas
    clockgen.setupPLL(SI5351_PLL_A, 36, 0, 1);  // 900 MHz
    
    // Banda de 20 metros - Frecuencia FT8 (14.074 MHz)
    uint32_t div_20m = 900000000UL / 14074000UL;
    clockgen.setupMultisynth(0, SI5351_PLL_A, div_20m, 0, 1);
    
    // Banda de 40 metros - Frecuencia PSK31 (7.07035 MHz)  
    uint32_t div_40m = 900000000UL / 7070350UL;
    clockgen.setupMultisynth(1, SI5351_PLL_A, div_40m, 0, 1);
    
    // Referencia de 10 MHz
    clockgen.setupMultisynth(2, SI5351_PLL_A, 90, 0, 1);
    
    Serial.println("Configuración radioaficionado aplicada");
    Serial.println("CLK0: 14.074 MHz (20m FT8)");
    Serial.println("CLK1: 7.070 MHz (40m PSK31)");
    Serial.println("CLK2: 10 MHz (referencia)");
}

// Ejemplo 2: Generador de señales de prueba
void setupTestSignals(Si5351& si5351) {
    // Señal de 1 kHz para audio
    si5351.set_freq(100000ULL, SI5351_CLK0);     // 1 kHz
    
    // Señal de 1 MHz para digital
    si5351.set_freq(100000000ULL, SI5351_CLK1);  // 1 MHz
    
    // Señal de 10 MHz para RF
    si5351.set_freq(1000000000ULL, SI5351_CLK2); // 10 MHz
    
    Serial.println("Señales de prueba configuradas");
}

// Ejemplo 3: Sweep de frecuencias
void frequencySweep(Si5351& si5351, unsigned long startFreq, unsigned long endFreq, int steps, int delayMs) {
    unsigned long step = (endFreq - startFreq) / steps;
    
    Serial.print("Iniciando sweep de ");
    Serial.print(startFreq);
    Serial.print(" Hz a ");
    Serial.print(endFreq);
    Serial.println(" Hz");
    
    for (int i = 0; i <= steps; i++) {
        unsigned long freq = startFreq + (step * i);
        si5351.set_freq(freq * 100ULL, SI5351_CLK0);
        
        Serial.print("Frecuencia: ");
        Serial.print(freq);
        Serial.println(" Hz");
        
        delay(delayMs);
    }
    
    Serial.println("Sweep completado");
}

// Ejemplo 4: Configuración de diferentes potencias
void setupDifferentPowers(Si5351& si5351) {
    // CLK0 con potencia baja (2mA)
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
    si5351.set_freq(100000000ULL, SI5351_CLK0); // 1 MHz
    
    // CLK1 con potencia media (4mA)
    si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_4MA);
    si5351.set_freq(200000000ULL, SI5351_CLK1); // 2 MHz
    
    // CLK2 con potencia alta (8mA)
    si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);
    si5351.set_freq(300000000ULL, SI5351_CLK2); // 3 MHz
    
    Serial.println("Diferentes potencias configuradas");
}

// Ejemplo 5: Quadrature (90 grados de desfase)
void setupQuadrature(Si5351& si5351, unsigned long frequency) {
    // Configurar dos salidas con 90 grados de desfase
    si5351.set_freq_manual(frequency * 100ULL, 0, SI5351_CLK0);
    si5351.set_phase(SI5351_CLK0, 0);   // 0 grados
    
    si5351.set_freq_manual(frequency * 100ULL, 0, SI5351_CLK1);
    si5351.set_phase(SI5351_CLK1, 127); // 90 grados (127 = máximo/2)
    
    // Sincronizar las salidas
    si5351.pll_reset(SI5351_PLLA);
    
    Serial.print("Señales en cuadratura a ");
    Serial.print(frequency);
    Serial.println(" Hz configuradas");
}

// Ejemplo 6: Calibración manual
void calibrateManual(Si5351& si5351, int32_t correction) {
    // Aplicar corrección de frecuencia (en partes por billón)
    si5351.set_correction(correction, SI5351_PLL_INPUT_XO);
    
    Serial.print("Corrección aplicada: ");
    Serial.print(correction);
    Serial.println(" ppb");
}

// Ejemplo 7: Monitoreo de estado
void monitorStatus(Si5351& si5351) {
    si5351.update_status();
    
    Serial.println("=== Estado del SI5351 ===");
    
    if (si5351.dev_status.SYS_INIT) {
        Serial.println("✓ Sistema inicializado");
    } else {
        Serial.println("✗ Sistema NO inicializado");
    }
    
    if (!si5351.dev_status.LOL_A) {
        Serial.println("✓ PLL A bloqueado");
    } else {
        Serial.println("✗ PLL A desbloqueado");
    }
    
    if (!si5351.dev_status.LOL_B) {
        Serial.println("✓ PLL B bloqueado");
    } else {
        Serial.println("✗ PLL B desbloqueado");
    }
    
    if (!si5351.dev_status.LOS) {
        Serial.println("✓ Oscilador externo OK");
    } else {
        Serial.println("✗ Pérdida de oscilador externo");
    }
}

#endif // SI5351_EXAMPLES_H
