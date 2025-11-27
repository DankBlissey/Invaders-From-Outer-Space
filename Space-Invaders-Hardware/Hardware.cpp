#include "Hardware.h"

// Write mem doesn't allow writes to ROM and handles mirrored addresses
void Hardware::writeMem(uint16_t addr, uint8_t data) {
    if (addr > ramStart) {
        (*mem)[ramStart + ((addr-ramStart) & ramMask)] = data;
    }
}
// Read mem handles mirrored addresses
uint8_t Hardware::readMem(uint16_t addr) {
    if (addr > ramStart) {
        return (*mem)[ramStart + ((addr - ramStart) & ramMask)];
    }
    return (*mem)[addr];
}
// Write mem with ability to write to ROM: For loading ROMs
void Hardware::loadMem(uint16_t addr, uint8_t data) {
    (*mem)[addr] = data;
}