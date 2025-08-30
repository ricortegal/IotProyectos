// Encender y apagar LED en GPIO 4 (pin digital 4)
#include <Arduino.h>

#define LED_PIN 4
#define DELAY_TIME 10

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH); // Enciende LED
    delay(DELAY_TIME);                // Espera DELAY_TIME
    digitalWrite(LED_PIN, LOW); // Apaga LED
    delay(DELAY_TIME);                // Espera DELAY_TIME
}
