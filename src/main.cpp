#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

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
unsigned long accumulatedMs = 0UL; // czas zmierzony przed ostatnim uruchomieniem
unsigned long startTime = 0UL;
unsigned long lastDisplayedSeconds = 0xFFFFFFFFUL;

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

// Aktualizuje tylko zmieniające się fragmenty ekranu, bez lcd.clear().
void updateDisplay() {
    unsigned long seconds = elapsedMs() / 1000UL;

    if (seconds != lastDisplayedSeconds) {
        lcd.setCursor(4, 1);
        printTwoDigits(seconds / 3600UL);
        lcd.print(':');
        printTwoDigits((seconds / 60UL) % 60UL);
        lcd.print(':');
        printTwoDigits(seconds % 60UL);
        lastDisplayedSeconds = seconds;
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
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Stoper");
    updateStatus();
    updateDisplay();
}

void loop() {
    // Oba przyciski muszą być sprawdzane w każdym obiegu, żeby śledzić ich stan.
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
