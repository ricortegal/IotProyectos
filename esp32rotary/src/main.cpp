#include <Arduino.h>
#include <Wire.h>
#include "Config.h"
#include "QuadratureEncoder.h"
#include "DebouncedButton.h"
#include "LcdUi.h"

QuadratureEncoder encoder;
DebouncedButton button;
LcdUi ui;
volatile bool lastAccel = false; // actualizado desde callback

void onNotch(int delta, bool accelerated) {
  lastAccel = accelerated;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  ui.begin(0x27);
  button.begin(PIN_ENC_BTN, true);
  encoder.setCallback(onNotch);
  encoder.begin();
  ui.showValue(encoder.value());
}

void loop() {
  button.update();
  if (button.wasLongPress()) {
    encoder.setValue(0);
  } else if (button.wasClick()) {
    encoder.setValue(encoder.value() + 10);
  }
  ui.showValue(encoder.value());
  ui.showStatus(button.isDown(), lastAccel);
  delay(5);
}