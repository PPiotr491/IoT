#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL
#define DISPLAY_REFRESH_PERIOD 50UL

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct DebouncedButton {
    int pin;
    int debouncedState;
    int previousReading;
    unsigned long lastChangeTime;
};

bool isPressed(DebouncedButton &button) {
    bool isPressed = false;
    int currentReading = digitalRead(button.pin);

    if (button.previousReading != currentReading) {
        button.lastChangeTime = millis();
    }

    if (millis() - button.lastChangeTime > DEBOUNCE_PERIOD) {
        if (currentReading != button.debouncedState) {
            if (button.debouncedState == HIGH && currentReading == LOW) {
                isPressed = true;
            }
            button.debouncedState = currentReading;
        }
    }

    button.previousReading = currentReading;

    return isPressed;
}

DebouncedButton redButton = {RED_BUTTON, HIGH, HIGH, 0UL};
DebouncedButton greenButton = {GREEN_BUTTON, HIGH, HIGH, 0UL};

bool running = false;
unsigned long accumulatedMs = 0UL;
unsigned long startTime = 0UL;
unsigned long lastDisplayedMs = 0xFFFFFFFFUL;
unsigned long lastRefreshTime = 0UL;

unsigned long elapsedMs() {
    if (running) {
        return accumulatedMs + (millis() - startTime);
    }
    return accumulatedMs;
}

void printTwoDigits(unsigned long value) {
    if (value < 10) {
        lcd.print('0');
    }
    lcd.print(value);
}

void printThreeDigits(unsigned long value) {
    if (value < 100) {
        lcd.print('0');
    }
    printTwoDigits(value);
}

// Czas jest liczony cały czas z millis(), a ekran odświeżany najwyżej co 50 ms.
void updateDisplay() {
    if (millis() - lastRefreshTime < DISPLAY_REFRESH_PERIOD) {
        return;
    }
    lastRefreshTime = millis();

    unsigned long ms = elapsedMs();

    if (ms != lastDisplayedMs) {
        lcd.setCursor(3, 1);
        printTwoDigits(ms / 60000UL);
        lcd.print(':');
        printTwoDigits((ms / 1000UL) % 60UL);
        lcd.print('.');
        printThreeDigits(ms % 1000UL);
        lastDisplayedMs = ms;
    }
}

void updateStatus() {
    lcd.setCursor(8, 0);
    lcd.print(running ? "START" : "STOP ");
}

void setup() {
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    lcd.init();
    lcd.clear();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Stoper");
    updateStatus();
    updateDisplay();
}

void loop() {
    bool greenPressed = isPressed(greenButton);
    bool redPressed = isPressed(redButton);

    if (greenPressed) {
        if (running) {
            accumulatedMs += millis() - startTime;
            running = false;
        } else {
            startTime = millis();
            running = true;
        }
        updateStatus();
    }

    if (redPressed) {
        running = false;
        accumulatedMs = 0UL;
        updateStatus();
    }

    updateDisplay();
}
