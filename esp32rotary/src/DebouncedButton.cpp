#include "DebouncedButton.h"

void DebouncedButton::update() {
    bool raw = readRaw();
    uint32_t now = millis();
    if (raw != _stable) {
        if (now - _lastChange >= BUTTON_DEBOUNCE_MS) {
            _stable = raw;
            _lastChange = now;
            if (_stable) { // press
                _pressStart = now;
            } else { // release
                uint32_t held = now - _pressStart;
                if (held >= BUTTON_LONGPRESS_MS) _long = true; else _click = true;
            }
        }
    } else {
        // no change
    }
}

bool DebouncedButton::wasClick() {
    if (_click) { _click = false; return true; }
    return false;
}

bool DebouncedButton::wasLongPress() {
    if (_long) { _long = false; return true; }
    return false;
}