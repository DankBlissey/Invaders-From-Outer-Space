#include "Hardware.h"

// Initialize object with relevant output port functions
Hardware::Hardware() {
    // Out Port 2 specifies the shift offset
    intel8080.setOutPort(2, [&](uint8_t offset){ shiftRegister.setOffset(offset); }); 
    // Out Port 4 shift a new 8 bit value into the register
    intel8080.setOutPort(4, [&](uint8_t shiftData){ shiftRegister.shiftValueIn(shiftData); }); 
    // In Port 3 gets the shifted value from the register
    intel8080.setInPort(3, [&](){ return shiftRegister.readShiftRegister(); });
}

void Hardware::frame() {
    unsigned long long halfEndpoint {totalFrames * cyclesPerHalfFrame};
    unsigned long long endpoint {totalFrames * cyclesPerFrame};
    while (totalCycles < halfEndpoint) {
        totalCycles += intel8080.cycle();
    }
    // insert frame rendering here as it allows for cpu init cycles to initialize vRam 
    // but it is before the CPU actually starts changing things in vRam via the interrupt
    intel8080.requestInterrupt(0xCF);
    while(totalCycles < endpoint) {
        totalCycles += intel8080.cycle();
    }
    intel8080.requestInterrupt(0xD7);
}