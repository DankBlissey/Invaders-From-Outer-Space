#ifndef INTEL_8080_STATE_H
#define INTEL_8080_STATE_H

#include "CPU.h"
#include <stack>

class Intel_8080_State {
	public:
		Intel_8080_State();
		void Step();
		void Backstep();
		size_t Size();
	private:
		std::stack<CPU> history;
	
};
#endif