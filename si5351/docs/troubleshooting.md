# Guía de solución de problemas

## Problemas comunes y soluciones

### 1. "Error: No se encontró el SI5351!"

**Posibles causas:**
- Conexiones I2C incorrectas
- Level shifter no habilitado
- Módulo SI5351 sin alimentación
- Dirección I2C incorrecta

**Soluciones:**
1. Verificar todas las conexiones según el esquema
2. Asegurarse de que OE del TXS0108E esté conectado a 3.3V
3. Verificar que el SI5351 reciba 3.3V
4. Probar con un escáner I2C para detectar dispositivos

### 2. LED parpadea constantemente

**Causa:** Error de inicialización del SI5351

**Solución:** Revisar las conexiones y la alimentación

### 3. Frecuencias incorrectas en la salida

**Posibles causas:**
- Cristal del SI5351 con frecuencia diferente a 25MHz
- Calibración necesaria

**Soluciones:**
1. Verificar la frecuencia del cristal del módulo
2. Ajustar el valor de corrección en el código si es necesario

### 4. No responde a comandos serie

**Posibles causas:**
- Velocidad de baudios incorrecta
- Buffer serie lleno

**Soluciones:**
1. Verificar que el monitor serie esté a 115200 baudios
2. Reiniciar el Arduino

### 5. Señales de salida débiles

**Causa:** Potencia de salida insuficiente

**Solución:** Cambiar la configuración de potencia:
```cpp
si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
```

## Escáner I2C para diagnóstico

Puedes usar este código para escanear dispositivos I2C:

```cpp
#include <Wire.h>

void scanI2C() {
  Serial.println("Escaneando dispositivos I2C...");
  int nDevices = 0;
  
  for(byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en dirección 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();
      nDevices++;
    }
  }
  
  if (nDevices == 0)
    Serial.println("No se encontraron dispositivos I2C");
  else
    Serial.println("Escaneo completado");
}
```

## Medición de frecuencias

Para verificar las frecuencias de salida, puedes usar:
- Osciloscopio
- Frecuencímetro
- Analizador de espectro
- Receptor SDR

## Consejos de uso

1. **Blindaje:** Usa cables cortos y considera blindaje para aplicaciones sensibles
2. **Filtros:** Para aplicaciones RF, añade filtros pasa-bajos en las salidas
3. **Referencia:** Para mayor precisión, usa un oscilador de referencia externo
4. **Carga:** Las salidas del SI5351 están diseñadas para cargas de 50Ω
