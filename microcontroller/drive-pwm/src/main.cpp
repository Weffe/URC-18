//
// Created by matthew on 24/08/17.
//

#include <Servo.h>
#include <Arduino.h>
#include <Wire.h>

Servo** servos;
uint8_t servoCount = 0;
uint8_t lastName = 0;

const int ADDRESS = 0x30;

uint8_t addServo(uint8_t pin) {
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

void setServo(uint8_t name, uint16_t micros) {
    if (name < servoCount) {
        servos[name]->writeMicroseconds(micros);
    }
}

void onRecieve(int bytes) {
    Serial.println("I getting data");
    int func = Wire.read();
    Serial.println(func);
    if (bytes == 2 && func == 0x01) {
        Serial.println("Adding pin");
        lastName = addServo(static_cast<uint8_t>(Wire.read()));
    }
    else if (bytes == 4 && func == 0x02) {
        Serial.println("Setting pin");
        uint8_t servo = static_cast<uint8_t>(Wire.read());
        uint8_t buf[2];
        Wire.readBytes(buf, 2);
        setServo(servo, reinterpret_cast<uint16_t>(buf));
    }
    else {
        bytes--;
        while (bytes > 0) {
            Wire.read(); // scoop it up
            bytes--;
        }
    }
}

void onRequest() {
    Serial.println("i need to send data");
    Wire.write(lastName);
}

void setup() {
    Serial.begin(9600); //debugging
    Serial.println("OK");
    Wire.begin(ADDRESS);
    Wire.onRequest(onRequest);
    Wire.onReceive(onRecieve);
    Serial.println("I am running");
}

void loop() {
    delay(100); // yaaaay
    Serial.println("I am loop");
}
