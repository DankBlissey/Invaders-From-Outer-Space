#include <catch2/catch_test_macros.hpp>
#include "Intel_8080_State.cpp"

TEST_CASE("CMC Complement Carry", "[opcodes, carry]") {
    CPU testCpu = CPU();
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
    CPU testCpu = CPU();
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