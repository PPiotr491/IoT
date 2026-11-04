#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define POTENTIOMETER A0
#define VREF 5.0

LiquidCrystal_I2C lcd(0x27, 16, 2);

int lastAdc = -1;

void setup() {
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("ADC:");
    lcd.setCursor(0, 1);
    lcd.print("U:");
}

void loop() {
    int adc = analogRead(POTENTIOMETER);

    if (adc != lastAdc) {
        lastAdc = adc;
        float voltage = VREF * adc / 1024.0;

        lcd.setCursor(5, 0);
        lcd.print(adc);
        lcd.print("    ");   // wyczyszczenie pozostałości po dłuższej liczbie

        lcd.setCursor(3, 1);
        lcd.print(voltage, 2);
        lcd.print(" V  ");
    }
}