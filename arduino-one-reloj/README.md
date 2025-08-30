# Proyecto PlatformIO: Arduino Uno - Encender y Apagar LED en GPIO 4

Este proyecto utiliza PlatformIO para programar un Arduino Uno y controlar un LED conectado al pin digital 4 (GPIO 4). El LED se encenderá y apagará en intervalos regulares.

## Requisitos
- Arduino Uno
- LED
- Resistencia (220Ω recomendada)
- Cables de conexión
- PlatformIO instalado en VS Code

## Conexión
- Conecta el ánodo del LED al pin digital 4 del Arduino Uno.
- Conecta el cátodo del LED a una resistencia y luego a GND.

## Compilación y carga
1. Abre el proyecto en VS Code con PlatformIO.
2. Compila el proyecto usando el botón "Build" de PlatformIO.
3. Carga el programa al Arduino Uno usando el botón "Upload".

## Código principal
El código se encuentra en `src/main.cpp` y realiza el encendido y apagado del LED cada segundo.

---
