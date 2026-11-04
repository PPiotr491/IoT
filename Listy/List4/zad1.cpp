#include <Arduino.h>

#define RED_BUTTON 2
#define GREEN_BUTTON 4

void initButtons() {
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup() {
    initButtons();

    Serial.begin(9600);
    while(!Serial) {
    };
}


void loop() {
    String str = String(digitalRead(RED_BUTTON)) + ", " + String(digitalRead(GREEN_BUTTON));
    Serial.println(str);
}