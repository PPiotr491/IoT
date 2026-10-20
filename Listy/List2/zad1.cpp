#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

const int colors[] = {LED_RED, LED_GREEN, LED_BLUE};
const int colorCount = sizeof(colors) / sizeof(colors[0]);

int currentColor = 0;
bool ledOn = true;

bool lastRedPressed = false;
bool lastGreenPressed = false;

void updateLeds() {
    for (int i = 0; i < colorCount; i++) {
        digitalWrite(colors[i], (ledOn && i == currentColor) ? HIGH : LOW);
    }
}

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    updateLeds();
}

void loop() {
    bool redPressed = digitalRead(RED_BUTTON) == LOW;
    bool greenPressed = digitalRead(GREEN_BUTTON) == LOW;

    if (greenPressed && !lastGreenPressed) {
        currentColor = (currentColor + 1) % colorCount;
        updateLeds();
    }

    if (redPressed && !lastRedPressed) {
        ledOn = !ledOn;
        updateLeds();
    }

    lastRedPressed = redPressed;
    lastGreenPressed = greenPressed;
}
