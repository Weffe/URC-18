//
// Created by matthew on 24/08/17.
//

#include "ard_device.h"

namespace rover_drive {

    // arduino protocol-ish

    // open pin: 0x01 0xXX where XX is the pin
    // set pin value 0x02 0xXX 0xYY 0xYY, XX is the pin, YY is an unsigned short being the pwm

    ARDevice::ARDevice(uint8_t busnum, uint8_t address) : device(busnum, address) {
        this->device.open_();
    }

    void ARDevice::openPin(uint8_t pin) {
        device.writeRegister(0x01, pin); // might rename this func, it should really be writeTwo
    }

    void ARDevice::writeMicroseconds(uint8_t pin, uint16_t microSeconds) {
        uint8_t packet[] = {
                0x02,
                pin,
                0x00,
                0x00
        };
        uint8_t *short_ = simpli2c::shortBuffer(microSeconds);
        packet[2] = short_[0];
        packet[3] = short_[1];
        device.writeMany(packet, 4);
    }
}