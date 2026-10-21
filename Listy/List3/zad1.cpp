#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

const int leds[] = {LED_RED, LED_GREEN, LED_BLUE};
const int ledCount = sizeof(leds) / sizeof(leds[0]);

int ledIndex = 0;

struct DebouncedButton {
    int pin;
    int debouncedState;
    int previousReading;
    unsigned long lastChangeTime;
};

DebouncedButton redButton = {RED_BUTTON, HIGH, HIGH, 0UL};
DebouncedButton greenButton = {GREEN_BUTTON, HIGH, HIGH, 0UL};

bool isReleased(DebouncedButton &button) {
    bool isReleased = false;
    int currentReading = digitalRead(button.pin);

    if (button.previousReading != currentReading) {
        button.lastChangeTime = millis();
    }

    if (millis() - button.lastChangeTime > DEBOUNCE_PERIOD) {
        if (currentReading != button.debouncedState) {
            if (button.debouncedState == LOW && currentReading == HIGH) {
                isReleased = true;
            }
            button.debouncedState = currentReading;
        }
    }

    button.previousReading = currentReading;

    return isReleased;
}

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void loop() {
    bool redReleased = isReleased(redButton);
    bool greenReleased = isReleased(greenButton);

    if (redReleased || greenReleased) {
        digitalWrite(leds[ledIndex], LOW);
        ledIndex = (ledIndex + 1) % ledCount;
        digitalWrite(leds[ledIndex], HIGH);
    }
}
