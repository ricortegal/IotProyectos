# Esquema de conexión detallado - SI5351 con Arduino Uno

## Diagrama principal de conexiones

```
┌─────────────────┐         ┌──────────────────┐         ┌─────────────────┐
│   Arduino Uno   │         │   TXS0108E       │         │   SI5351        │
│                 │         │   Level Shifter  │         │   Module        │
├─────────────────┤         ├──────────────────┤         ├─────────────────┤
│                 │         │                  │         │                 │
│ 5V       ○──────┼─────────┼─○ VCCA (1)       │         │                 │
│ GND      ○──────┼─────────┼─○ GND  (10)      │    ┌────┼─○ GND           │
│                 │         │                  │    │    │                 │
│ A4 (SDA) ○──────┼─────────┼─○ A1   (2)       │    │    │                 │
│ A5 (SCL) ○──────┼─────────┼─○ A2   (3)       │    │    │                 │
│                 │         │                  │    │    │                 │
│ 3.3V     ○──────┼─────────┼─○ VCCB (20) ○────┼────┼────┼─○ VCC (3.3V)    │
│                 │         │          ○       │    │    │                 │
│                 │         │        OE (11)   │    │    │                 │
│                 │         │          ○───────┼────┘    │                 │
│                 │         │                  │         │                 │
│                 │         │          B1 (19) ○─────────┼─○ SDA           │
│                 │         │          B2 (18) ○─────────┼─○ SCL           │
│                 │         │                  │         │                 │
│                 │         │                  │         │ CLK0     ○──────┼── Salida 1
│                 │         │                  │         │ CLK1     ○──────┼── Salida 2  
│                 │         │                  │         │ CLK2     ○──────┼── Salida 3
└─────────────────┘         └──────────────────┘         └─────────────────┘
```

## Tabla de conexiones

| Arduino Uno | Pin TXS0108E | Función      | Pin SI5351 | Función |
|-------------|--------------|--------------|------------|---------|
| 5V          | VCCA (1)     | Alimentación | -          | -       |
| GND         | GND (10)     | Tierra       | GND        | Tierra  |
| A4 (SDA)    | A1 (2)       | I2C Data     | -          | -       |
| A5 (SCL)    | A2 (3)       | I2C Clock    | -          | -       |
| 3.3V        | VCCB (20)    | Alimentación | VCC        | 3.3V    |
| -           | OE (11)      | Enable       | 3.3V       | Enable  |
| -           | B1 (19)      | I2C Data     | SDA        | I2C Data|
| -           | B2 (18)      | I2C Clock    | SCL        | I2C Clock|

## Notas importantes

### ⚠️ Advertencias de conexión
- **NUNCA** conectes directamente 5V del Arduino al SI5351
- El pin OE del TXS0108E **DEBE** estar conectado a 3.3V para funcionar
- Verifica que tu módulo SI5351 tenga regulador de 3.3V integrado

### 🔍 Verificación de conexiones
1. **Continuidad**: Usa un multímetro para verificar conexiones
2. **Voltajes**: Mide que el SI5351 reciba exactamente 3.3V
3. **I2C**: Puedes usar un escáner I2C para detectar el dispositivo

### 📍 Pinout del TXS0108E (vista superior)
```
      ┌─────────────────────┐
   1──┤ VCCA          VCCB ├──20
   2──┤ A1              B1 ├──19
   3──┤ A2              B2 ├──18
   4──┤ A3              B3 ├──17
   5──┤ A4              B4 ├──16
   6──┤ A5              B5 ├──15
   7──┤ A6              B6 ├──14
   8──┤ A7              B7 ├──13
   9──┤ A8              B8 ├──12
  10──┤ GND             OE ├──11
      └─────────────────────┘
```

### 🎯 Salidas del SI5351
- **CLK0**: Salida configurable 1 (frecuencia programable)
- **CLK1**: Salida configurable 2 (frecuencia programable)  
- **CLK2**: Salida configurable 3 (frecuencia programable)

### 📊 Especificaciones técnicas
- **Rango de frecuencia**: 8 kHz - 160 MHz
- **Resolución**: <0.1 Hz en frecuencias bajas
- **Impedancia de salida**: 50Ω (diseñada para cargas de 50Ω)
- **Niveles de salida**: CMOS 3.3V
- **Corriente máxima por salida**: 8mA

### 🔧 Resolución de problemas
- Si no detecta el SI5351: revisar conexiones I2C y alimentación
- Si las frecuencias son incorrectas: verificar cristal de referencia (25MHz)
- Si no hay salida: verificar que las salidas estén habilitadas en código
