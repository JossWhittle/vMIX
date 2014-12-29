#pragma once

// std Includes
#include <chrono>
#include <thread>
#include <iostream>

// vMIX Includes
#include "Mem.hpp"
#include "OpCode.hpp"
#include "Instruction.hpp"

using namespace std;

// How much memory does 
// the system have in words
constexpr uint MEM_SIZE = 4000;

class Machine {
private:
	// Comparison Indicator
	enum class CMP_FLAG {
		LESS,
		EQUAL,
		GREATER
	} CI = CMP_FLAG::EQUAL;

	// Overflow Indicator
	enum class OF_FLAG {
		OFF,
		ON
	} OF = OF_FLAG::OFF;

	// Registers
	vWord rA{}, rX{}, rI1{}, rI2{}, rI3{}, rI4{}, rI5{}, rI6{}, rJ{};

	// Internal RAM
	vWord memory[MEM_SIZE]{};

	// CPU Frequency
	uint  tik;
	vWord instrPtr{};

	// Helpers
	inline void sleep(const uint t) const {
		this_thread::sleep_for(chrono::milliseconds(t * tik));
	};

public:
	// Default Constructor
	Machine(uint _tik) : tik(_tik) {
		INSTR::setC(memory[instrPtr], (uint) OP::SPECIAL);
		INSTR::setF(memory[instrPtr], (uint) FIELD_SPECIAL::HLT);

		while (instrPtr >= 0 && instrPtr < MEM_SIZE) {
			const vWord &instr = memory[instrPtr];
			if (eval(instr)) break;
		}
	};

	inline bool eval(const vWord &instr) {
		const uint C = INSTR::getC(instr);
		const uint F = INSTR::getF(instr);
		const uint I = INSTR::getI(instr);

		// Sleep for base op time
		sleep(OP_TIME[C]);

		switch ((OP)C) {
			case OP::NOP: break;
			
			// Arithmetic
			case OP::ADD: {
				break;
			}
			case OP::SUB: {
				break;
			}
			case OP::MUL: {
				break;
			}
			case OP::DIV: {
				break;
			}

			// Special
			case OP::SPECIAL: {
				if (F == (uint) FIELD_SPECIAL::HLT) {
					cout << "HALT\n";
					return true;
				}
				break;
			}

			// Move
			case OP::MOVE: {
				break;
			}
		};

		instrPtr.data++;
		return false;
	};
};