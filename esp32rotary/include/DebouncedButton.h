#pragma once
#include <Arduino.h>
#include "Config.h"

class DebouncedButton {
public:
    void begin(int pin, bool pullup = true) {
        _pin = pin;
        if (pullup) pinMode(_pin, INPUT_PULLUP); else pinMode(_pin, INPUT);
        _stable = readRaw();
        _lastChange = millis();
    }
    void update();
    bool wasClick();       // pulsación corta liberada
    bool wasLongPress();   // pulsación larga liberada
    bool isDown() const { return _stable; }
private:
    bool readRaw() const { return digitalRead(_pin) == LOW; } // activo LOW
    int _pin = -1;
    bool _stable = false;
    bool _prevStable = false;
    uint32_t _lastChange = 0;
    uint32_t _pressStart = 0;
    bool _click = false;
    bool _long = false;
};