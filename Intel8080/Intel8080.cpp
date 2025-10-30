// Intel8080.cpp : Defines the entry point for the application.
//

#include "Intel8080.h"
#include <iostream>
#include "CPU.h"

int main()
{
	std::cout << "Hello World!\n";
	CPU cpu;

	cpu.init();
	cpu.loadTestProgram();
	cpu.status();
	cpu.cycle();
	cpu.status();
	std::cout << std::hex << unsigned(cpu.readMem(0x3A7C));
	return 0;
}
