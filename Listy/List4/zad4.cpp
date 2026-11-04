#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define POTENTIOMETER A0
#define VREF 5.0

void setup() {
    // Czyszczenie LCD - sterownik nie resetuje się razem z płytką.
    lcd.init();
    lcd.clear();
    lcd.noDisplay();
    lcd.noBacklight();

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
