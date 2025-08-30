# Proyecto SI5351 con Arduino Uno
## Control de generador de frecuencias SI5351 usando level shifter TXS0108E

### Descripción
Este proyecto permite controlar un módulo SI5351 (generador de frecuencias de hasta 160MHz) usando un Arduino Uno a través de un level shifter TXS0108E para adaptar los niveles de voltaje de 5V a 3.3V.

**Librería utilizada:** Adafruit Si5351 Library v1.4.2

### Componentes necesarios
- Arduino Uno
- Módulo SI5351 (breakout board)
- Level shifter TXS0108E
- Cables jumper
- Protoboard (opcional)

### Esquema de conexión

```
Arduino Uno (5V)        TXS0108E Level Shifter        SI5351 Module (3.3V)
================        ======================        ===================

Pin 5V      ----------->  VCCA (Pin 1)
Pin GND     ----------->  GND  (Pin 3)
                         VCCB (Pin 20) <-------------- 3.3V (del SI5351 o fuente externa)
                         GND  (Pin 10) <-------------- GND

Pin A4 (SDA) ----------> A1   (Pin 2)
Pin A5 (SCL) ----------> A2   (Pin 3)
                         B1   (Pin 19) <-------------- SDA (SI5351)
                         B2   (Pin 18) <-------------- SCL (SI5351)

Pin 3.3V    ----------->                               VCC (SI5351)
Pin GND     ----------->                               GND (SI5351)
```

### Pinout detallado del TXS0108E

```
TXS0108E Level Shifter (Vista superior)
========================================

    1  [VCCA] ---- 5V del Arduino
    2  [A1]   ---- SDA del Arduino (Pin A4)
    3  [A2]   ---- SCL del Arduino (Pin A5)
    4  [A3]   ---- (No usado)
    5  [A4]   ---- (No usado)
    6  [A5]   ---- (No usado)
    7  [A6]   ---- (No usado)
    8  [A7]   ---- (No usado)
    9  [A8]   ---- (No usado)
   10  [GND]  ---- GND común
   
   20  [VCCB] ---- 3.3V del SI5351
   19  [B1]   ---- SDA del SI5351
   18  [B2]   ---- SCL del SI5351
   17  [B3]   ---- (No usado)
   16  [B4]   ---- (No usado)
   15  [B5]   ---- (No usado)
   14  [B6]   ---- (No usado)
   13  [B7]   ---- (No usado)
   12  [B8]   ---- (No usado)
   11  [OE]   ---- Conectar a VCCB (3.3V) para habilitar
```

### Notas importantes de conexión

1. **Alimentación del SI5351**: El módulo SI5351 funciona a 3.3V. Asegúrate de que tu módulo tenga regulador de voltaje incorporado o usa una fuente de 3.3V separada.

2. **OE (Output Enable)**: El pin OE del TXS0108E debe conectarse a VCCB (3.3V) para habilitar el level shifter.

3. **Pull-ups**: Los pines I2C del Arduino ya tienen pull-ups internos, pero algunos módulos SI5351 también los incluyen. Esto generalmente no causa problemas.

4. **Frecuencias de salida**: El SI5351 tiene 3 salidas (CLK0, CLK1, CLK2) que puedes usar según tus necesidades.

### Comandos disponibles por puerto serie

- `0 <frecuencia>` - Configurar CLK0 (ejemplo: `0 1000000` para 1MHz)
- `1 <frecuencia>` - Configurar CLK1 
- `2 <frecuencia>` - Configurar CLK2
- `s` - Mostrar estado actual
- `r` - Reset a valores por defecto

### Rango de frecuencias
- Mínimo: 8 kHz
- Máximo: 160 MHz

### Ejemplo de uso
```
0 14074000    // Configurar CLK0 a 14.074 MHz (banda de 20m)
1 7074000     // Configurar CLK1 a 7.074 MHz (banda de 40m)
2 10000000    // Configurar CLK2 a 10 MHz (referencia)
s             // Mostrar estado
```

### Compilación e instalación

1. Instalar PlatformIO
2. Abrir el proyecto: `pio run`
3. Subir al Arduino: `pio run --target upload`
4. Monitor serie: `pio device monitor`
