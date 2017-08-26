//
// Created by matthew on 24/08/17.
//

#include "ard_device.h"

namespace rover_drive {

    // arduino protocol-ish

    // open pin: 0x01 0xXX where XX is the pin
    // set pin value 0x02 0xXX 0xYY 0xYY, XX is the pin, YY is an unsigned short being the pwm
    // reset: 0x03

    ARDevice::ARDevice(uint8_t busnum, uint8_t address) : device(busnum, address), pinMap() {
        this->device.open_();
        this->device.writeOne(0x03); // reset
    }

    void ARDevice::openPin(uint8_t pin) {
        uint8_t servoName = device.requestOne(new uint8_t[2] {0x01, pin}, 2);
        this->pinMap[pin] = servoName;
    }

    void ARDevice::writeMicroseconds(uint8_t pin, uint16_t microSeconds) {
        uint8_t packet[] = {
                0x02,
                this->pinMap[pin],
                0x00,
                0x00
        };
        uint8_t *short_ = simpli2c::shortBuffer(microSeconds);
        packet[2] = short_[0];
        packet[3] = short_[1];
        device.writeMany(packet, 4);
    }
}