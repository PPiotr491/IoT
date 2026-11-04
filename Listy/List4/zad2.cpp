#include <Arduino.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);

    Serial.begin(9600);
    while(!Serial) {
    };
}


// Returns true if the string is a non-empty sequence of digits with value 0-255
bool isValidValue(const String &s) {
    if (s.length() == 0 || s.length() > 3)
        return false;
    
    for (unsigned int i = 0; i < s.length(); i++) {
        if (!isDigit(s[i]))
            return false;
    }
    
    return s.toInt() <= 255;
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();

        if (command.startsWith("setled ")) {
            int firstSpace = command.indexOf(' ');
            int secondSpace = command.indexOf(' ', firstSpace + 1);
            int thirdSpace = command.indexOf(' ', secondSpace + 1);


            String redStr, greenStr, blueStr;
            // sprawdzenie czy jest wystarczająco argumentów i czy nie ma ich zbyt dużo
            if (secondSpace != -1 && thirdSpace != -1 && command.indexOf(' ', thirdSpace + 1) == -1) {
                redStr = command.substring(firstSpace + 1, secondSpace);
                greenStr = command.substring(secondSpace + 1, thirdSpace);
                blueStr = command.substring(thirdSpace + 1);
            }

            // sprawdzenie czy arugmenty są poprawne
            if (isValidValue(redStr) && isValidValue(greenStr) && isValidValue(blueStr)) {
                analogWrite(LED_RED, redStr.toInt());
                analogWrite(LED_GREEN, greenStr.toInt());
                analogWrite(LED_BLUE, blueStr.toInt());
                Serial.println("LED set to R=" + redStr + " G=" + greenStr + " B=" + blueStr);
            } else {
                Serial.println("Invalid arguments. Usage: setled R G B (values 0-255)");
            }
        }
        else {
            Serial.println("Unknown command '" + command + "'");
        }
    }
}