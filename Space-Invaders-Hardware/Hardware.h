#include "../Intel8080/CPU.h"

class Hardware : CPU {
    public: 
        void writeMem(uint16_t, uint8_t);
        uint8_t readMem(uint16_t);
        void loadMem(uint16_t, uint8_t);
    private:
        void writeOut(uint8_t, uint8_t);
        const uint16_t romStart {0x0000};
        const uint16_t romEnd {0x1FFF};
        const uint16_t ramStart {0x2000};
        const uint16_t ramEnd {0x23FF};
        const uint16_t vRamStart {0x2400};
        const uint16_t vRamEnd {0x3FFF};
        const uint16_t ramSize {static_cast<uint16_t>(vRamEnd - ramStart + 1)};
        const uint16_t ramMask {static_cast<uint16_t>(ramSize - 1)};
};