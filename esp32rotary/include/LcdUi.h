#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LcdUi {
public:
    void begin(uint8_t addr = 0x27) {
        _lcd = new LiquidCrystal_I2C(addr, 16, 2);
        _lcd->init();
        _lcd->backlight();
        _lcd->clear();
        _lcd->print("Init...");
    }
    void showValue(int v) {
        if (v != _lastValue) {
            _lcd->setCursor(0,0);
            _lcd->print("Valor:        ");
            _lcd->setCursor(7,0);
            _lcd->print(v);
            _lastValue = v;
        }
    }
    void showStatus(bool btn, bool accel) {
        _lcd->setCursor(0,1);
        if (btn) _lcd->print("BTN:DOWN "); else _lcd->print("BTN:OK   ");
        _lcd->setCursor(10,1);
        if (accel) _lcd->print("FAST"); else _lcd->print("    ");
    }
private:
    LiquidCrystal_I2C *_lcd = nullptr;
    int _lastValue = -99999;
};