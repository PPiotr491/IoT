#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

struct BlinkTask {
    int pin;
    unsigned long period;
    int state;
    unsigned long lastChange;
};

BlinkTask blinkTasks[] = {
    {LED_RED, 900UL, LOW, 0UL},
    {LED_GREEN, 1000UL, LOW, 0UL},
    {LED_BLUE, 1100UL, LOW, 0UL},
};
const int blinkTaskCount = sizeof(blinkTasks) / sizeof(blinkTasks[0]);

void run(BlinkTask &task) {
    if (millis() - task.lastChange >= task.period) {
        task.state = (task.state == HIGH) ? LOW : HIGH;
        digitalWrite(task.pin, task.state);
        task.lastChange += task.period;
    }
}

void setup() {
    lcd.init();
    lcd.clear();
    lcd.noDisplay();
    lcd.noBacklight();

    for (int i = 0; i < blinkTaskCount; i++) {
        pinMode(blinkTasks[i].pin, OUTPUT);
        digitalWrite(blinkTasks[i].pin, blinkTasks[i].state);
    }
}

void loop() {
    for (int i = 0; i < blinkTaskCount; i++) {
        run(blinkTasks[i]);
    }


}
