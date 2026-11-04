#include <Arduino.h>

#define POTENTIOMETER A0
#define VREF 5.0

void setup() {
    Serial.begin(9600);
    while (!Serial) {
    }
}

void loop() {
    int adc = analogRead(POTENTIOMETER);
    float voltage = VREF * adc / 1024.0;

    // Format "etykieta:wartość" nadaje nazwy seriom w legendzie.
    Serial.print("ADC:");
    Serial.print(adc);
    Serial.print(", U[V]:");
    Serial.println(voltage, 3);
}
