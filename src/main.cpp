#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define FADE_INTERVAL_MS 10

const int colors[] = {LED_RED, LED_GREEN, LED_BLUE};
const int colorCount = sizeof(colors) / sizeof(colors[0]);

int currentColor = 0;

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    analogWrite(LED_RED, 255);
    analogWrite(LED_GREEN, 0);
    analogWrite(LED_BLUE, 0);
}

void loop() {
    int from = colors[currentColor];
    int to = colors[(currentColor + 1) % colorCount];

    for (int i = 0; i <= 255; i++) {
        analogWrite(from, 255 - i);
        analogWrite(to, i);
        delay(FADE_INTERVAL_MS);
    }

    currentColor = (currentColor + 1) % colorCount;
}
