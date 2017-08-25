//
// Created by matthew on 24/08/17.
//

#include <Servo.h>
#include <Arduino.h>

Servo** servos;
uint8_t servoCount = 0;

uint8_t addServo(uint16_t pin) {
    if (servoCount != 0) {
        servos = static_cast<Servo **>(realloc(servos, sizeof(Servo *) * servoCount + 1));
    }
    else {
        servos = static_cast<Servo **>(malloc(sizeof(Servo *)));
    }
    servos[servoCount] = new Servo();
    servos[servoCount]->attach((int)pin);
    servoCount += 1;
    return servoCount - 1;
}

void setup() {

}

void loop() {

}