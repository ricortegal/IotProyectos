/*
 * Ejemplo simple para probar el SI5351
 * ===================================== 
 * 
 * Este código genera 3 frecuencias fijas para probar que 
 * el SI5351 está funcionando correctamente.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SI5351.h>

Adafruit_SI5351 clockgen = Adafruit_SI5351();

void setup() {
  Serial.begin(115200);
  Serial.println("=== Prueba simple SI5351 ===");
  
  // Inicializar I2C
  Wire.begin();
  
  // Inicializar SI5351
  if (!clockgen.begin()) {
    Serial.println("Error: SI5351 no encontrado!");
    while(1) delay(1000);
  }
  
  Serial.println("SI5351 encontrado!");
  
  // Configurar PLL A a 900 MHz
  clockgen.setupPLL(SI5351_PLL_A, 36, 0, 1);
  
  // CLK0: 1 MHz (900MHz / 900)
  clockgen.setupMultisynth(0, SI5351_PLL_A, 900, 0, 1);
  
  // CLK1: 2 MHz (900MHz / 450) 
  clockgen.setupMultisynth(1, SI5351_PLL_A, 450, 0, 1);
  
  // CLK2: 10 MHz (900MHz / 90)
  clockgen.setupMultisynth(2, SI5351_PLL_A, 90, 0, 1);
  
  // Habilitar todas las salidas
  clockgen.enableOutputs(true);
  
  Serial.println("Frecuencias configuradas:");
  Serial.println("CLK0: 1 MHz");
  Serial.println("CLK1: 2 MHz"); 
  Serial.println("CLK2: 10 MHz");
  Serial.println("¡Proyecto listo para usar!");
}

void loop() {
  // Parpadear LED para indicar que funciona
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  
  // Enviar estado cada 5 segundos
  static unsigned long lastReport = 0;
  if (millis() - lastReport > 5000) {
    Serial.println("Sistema funcionando correctamente...");
    lastReport = millis();
  }
}
