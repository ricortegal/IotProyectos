#pragma once
#include <Arduino.h>
#include "Config.h"

class QuadratureEncoder {
public:
    using StepCallback = void(*)(int delta, bool accelerated);
    void begin();
    int  value() const { return _value; }
    void setValue(int v);
    void setCallback(StepCallback cb) { _cb = cb; }

#ifndef USE_SIMPLE_ENCODER
    // Avanzado
    void handleISR(); // llamado desde ambas interrupciones
#endif

private:
    volatile int _value = 0;
    StepCallback _cb = nullptr;

#ifndef USE_SIMPLE_ENCODER
    volatile int _subAccum = 0;
    volatile uint32_t _lastNotchUs = 0;
    volatile uint8_t _lastState = 0; // (A<<1)|B
#else
    // Modo simple: usamos acumulador de semi-pasos y estado previo
    volatile int _subAccum = 0;      // acumula +/-1 transiciones válidas, se confirma en +-2
    volatile uint8_t _lastState = 0; // estado actual (A<<1)|B
    friend void IRAM_ATTR encoderIsrCLK();
    friend void IRAM_ATTR encoderIsrDT();
    friend void IRAM_ATTR processSimpleISR();
#endif
};

#ifdef USE_SIMPLE_ENCODER
void IRAM_ATTR encoderIsrCLK();
void IRAM_ATTR encoderIsrDT();
void IRAM_ATTR processSimpleISR();
#else
void IRAM_ATTR encoderIsrCLK();
void IRAM_ATTR encoderIsrDT();
#endif