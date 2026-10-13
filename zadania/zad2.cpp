#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define SELECTED_LED LED_GREEN

#define FADE_INTERVAL_MS 10


unsigned long lastFadeTime = 0;
int brightness = 0;

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    analogWrite(SELECTED_LED, brightness);
}

void loop() {
    bool greenPressed = digitalRead(GREEN_BUTTON) == LOW;
    bool redPressed = digitalRead(RED_BUTTON) == LOW;

    if (greenPressed && !redPressed && brightness < 255) {
        brightness++;
    } else if (redPressed && !greenPressed && brightness > 0) {
        brightness--;
    }

    analogWrite(SELECTED_LED, brightness);

    delay(FADE_INTERVAL_MS);
}
