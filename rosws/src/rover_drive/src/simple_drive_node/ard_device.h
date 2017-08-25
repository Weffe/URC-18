//
// Created by matthew on 24/08/17.
//

#ifndef PROJECT_ARD_DEVICE_H
#define PROJECT_ARD_DEVICE_H

#include <simpli2c/simpli2c.h>

namespace rover_drive {

    class ARDevice {
    public:
        ARDevice(uint8_t busnum, uint8_t address);

        void openPin(uint8_t pin);
        void writeMicroseconds(uint8_t pin, uint16_t microSeconds);
    private:
        simpli2c::Device device;
    };

}


#endif //PROJECT_ARD_DEVICE_H
