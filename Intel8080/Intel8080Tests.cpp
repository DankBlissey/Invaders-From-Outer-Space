#include <catch2/catch_test_macros.hpp>
#include "Intel_8080_State.h"
#include <bit>
#include <cstdint>

TestCPU testCpu = TestCPU();

TEST_CASE("CMC Complement Carry", "[opcodes, carry]") {
    testCpu.init();
    testCpu.writeMem(0x000, 0x3F);
    testCpu.writeMem(0x001, 0x3F);
    // Carry begins at 0
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(0).stateEquals(testCpu));
    testCpu.cycle();
    // CMC flips from 0 to 1
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(1).stateEquals(testCpu));
    testCpu.cycle();
    // CMC flips back from 1 to 0
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(2).stateEquals(testCpu));
}

TEST_CASE("STC Set Carry", "[opcodes, carry]") {
    testCpu.init();
    testCpu.writeMem(0x000, 0x37);
    testCpu.writeMem(0x001, 0x37);
    // Carry begins at 0
    REQUIRE(Intel_8080_State().with_Carry(0).with_pc(0).stateEquals(testCpu));
    testCpu.cycle();
    // STC sets carry to 1
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(1).stateEquals(testCpu));
    testCpu.cycle();
    // STC does nothing, carry is already at 1
    REQUIRE(Intel_8080_State().with_Carry(1).with_pc(2).stateEquals(testCpu));
}

// Zero, Sign, Parity, AuxCarry flags affected
// 0-3 4 and C
TEST_CASE("INR Increment Register or Memory (values)", "[opcodes, singleRegInstruction, values]") {
    testCpu.init();
    SECTION("INR on basic registers") {
        testCpu.writeMem(0x000, 0x04); // INRB
        testCpu.writeMem(0x001, 0x0C); // INRC
        testCpu.writeMem(0x002, 0x14); // INRD
        testCpu.writeMem(0x003, 0x1C); // INRE
        testCpu.writeMem(0x004, 0x24); // INRH
        testCpu.writeMem(0x005, 0x2C); // INRL
        testCpu.writeMem(0x006, 0x3C); // INRA
        SECTION("INR correctly increments a register value") {
            testCpu.setAllReg(34);
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            REQUIRE(Intel_8080_State().with_B(35).with_C(35).with_D(35).with_E(35).with_H(35).with_L(35).with_A(35).stateEquals(testCpu));
        }
        SECTION("INR correctly increments negative numbers") {

            int8_t negative = -12;
            testCpu.setAllReg(std::bit_cast<uint8_t, int8_t>(negative));
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            int8_t inrNegative = -11;
            uint8_t inrConverted= std::bit_cast<uint8_t>(inrNegative);
            REQUIRE(Intel_8080_State().with_B(inrConverted).with_C(inrConverted).with_D(inrConverted).with_E(inrConverted).with_H(inrConverted).with_L(inrConverted).with_A(inrConverted).stateEquals(testCpu));
        }
        SECTION("INR correctly overflows") {
            int8_t maxNegative = -1;
            uint8_t notSigned = std::bit_cast<uint8_t>(maxNegative);
            testCpu.setAllReg(notSigned);
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            REQUIRE(Intel_8080_State().with_B(0).with_C(0).with_D(0).with_E(0).with_H(0).with_L(0).with_A(0).stateEquals(testCpu));
        }
    }
    SECTION("INR on memory location") {
        testCpu.writeMem(0x00,0x34); // INRM
        testCpu.setH(0x3A);
        testCpu.setL(0x7C);
        SECTION("INR correctly increments a memory value") {
            testCpu.writeMem(0x3A7C, 34);
            testCpu.cycle();
            REQUIRE(testCpu.readMem(0x3A7C) == 35);
        }
        SECTION("INR correctly increments a negative memory value") {
            int8_t negative = -12;
            testCpu.writeMem(0x3A7C, std::bit_cast<uint8_t>(negative)); 
            testCpu.cycle();
            int8_t inrNegative = -11;
            REQUIRE(testCpu.readMem(0x3A7C) == std::bit_cast<uint8_t>(inrNegative));
        }
        SECTION("INR correctly overflows") {
            int8_t maxNegative = -1;
            uint8_t notSigned = std::bit_cast<uint8_t>(maxNegative);
            testCpu.writeMem(0x3A7C, notSigned);
            testCpu.cycle();
            REQUIRE(testCpu.readMem(0x3A7C) == 0);
        }
    }
}

TEST_CASE("INR Increment Register or Memory (flags)", "[opcodes, singleRegInstruction, flags]") {
    testCpu.init();
    testCpu.writeMem(0x000, 0x04); // INRB
    SECTION("Case 1: -1 -> 0") {
        testCpu.setB(255);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 0);
        REQUIRE(testCpu.getZero() == true);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getCarry() == false);
    }
    SECTION("Case 2: 79 -> 80") {
        testCpu.setB(79);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 80);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
    SECTION("Case 3: 78 -> 79") {
        testCpu.setB(78);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 79);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == false);
    }
    SECTION("Case 4: 127 -> -128") {
        testCpu.setB(127);
        testCpu.cycle();
        int8_t minus = -128;
        REQUIRE(testCpu.getB() == std::bit_cast<uint8_t>(minus));
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getCarry() == false);
    }
}

TEST_CASE("DCR Decrement Register or Memory (values)", "[opcodes, singleRegInstructions, values]") {
    testCpu.init();
    SECTION("DCR on basic registers") {
        testCpu.writeMem(0x000, 0x05); // DCRB
        testCpu.writeMem(0x001, 0x0D); // DCRC
        testCpu.writeMem(0x002, 0x15); // DCRD
        testCpu.writeMem(0x003, 0x1D); // DCRE
        testCpu.writeMem(0x004, 0x25); // DCRH
        testCpu.writeMem(0x005, 0x2D); // DCRL
        testCpu.writeMem(0x006, 0x3D); // DCRA
        SECTION("DCR correctly decrements on all registers") {
            testCpu.setAllReg(34);
            for (int i = 0; i < 7; i++) {
                testCpu.cycle();
            }
            REQUIRE(Intel_8080_State().with_B(33).with_C(33).with_D(33).with_E(33).with_H(33).with_L(33).with_A(33).stateEquals(testCpu));
        }
    }
    SECTION("DCR on memory") {
        testCpu.writeMem(0x00,0x35); // DCRM
        testCpu.setH(0x3A);
        testCpu.setL(0x7C);
        SECTION("DCR correctly decrements a memory value") {
            testCpu.writeMem(0x3A7C, 34);
            testCpu.cycle();
            REQUIRE(testCpu.readMem(0x3A7C) == 33);
        }
    }
}

TEST_CASE("DCR Decrement Register or Memory (flags)", "[opcodes, singleRegInstructions, flags]") {
    testCpu.init();
    testCpu.writeMem(0x000, 0x05); // DCRB
    SECTION("Case 1: 80 -> 79") {
        testCpu.setB(80);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 79);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
    SECTION("Case 2: 1 -> 0") {
        testCpu.setB(1);
        testCpu.cycle();
        REQUIRE(testCpu.getB() == 0);
        REQUIRE(testCpu.getZero() == true);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == false);
    }
    SECTION("Case 3: 0 -> -1") {
        testCpu.setB(0);
        testCpu.cycle();
        int8_t minus = -1;
        REQUIRE(testCpu.getB() == std::bit_cast<uint8_t>(minus));
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getParity() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
    }
}

TEST_CASE("CMA Complement Accumulator", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    testCpu.writeMem(0x000, 0x2F); // CMA
    SECTION("Case 1:") {
        testCpu.setA(0b01001101);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b10110010);
    }
    SECTION("Case 2:") {
        testCpu.setA(0b10010111);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b01101000);
    }
    SECTION("Case 3:") {
        testCpu.setA(0b11111111);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b00000000);
    }
}

TEST_CASE("DAA Decimal Adjust Accumulator", "[opcodes, singleRegInstructions]") {
    testCpu.init();
    testCpu.writeMem(0x000, 0x27);
    SECTION("Case: 1, Provided in 8080 manual") {
        testCpu.setA(0x9B);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 1);
        REQUIRE(testCpu.getCarry() == true);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
    }
    SECTION("Case: 2") {
        testCpu.setA(1);
        testCpu.setAuxCarry(true);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 7);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
    }
    SECTION("Case: 3") {
        testCpu.setA(0b00110101);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 53);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == false);
        REQUIRE(testCpu.getSign() == false);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == true);
    }
    SECTION("Case: 4") {
        testCpu.setA(0b01111101);
        testCpu.cycle();
        REQUIRE(testCpu.getA() == 0b10000011);
        REQUIRE(testCpu.getCarry() == false);
        REQUIRE(testCpu.getAuxCarry() == true);
        REQUIRE(testCpu.getSign() == true);
        REQUIRE(testCpu.getZero() == false);
        REQUIRE(testCpu.getParity() == false);
    }
}