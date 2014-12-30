#pragma once

// std Includes
#include <chrono>
#include <thread>

// vMIX Includes
#include "Debug.hpp"
#include "Mem.hpp"
#include "OpCode.hpp"
#include "Instruction.hpp"
#include "IODevice.hpp"

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

	// IO Devices
	constexpr static uint NUM_DEVICES = 21;
	IODevice *devices[NUM_DEVICES];

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

		// Binary Storage
		for (int i = 0; i <= 7; ++i) {
			devices[i] = new IOTape("data/TapeUnit-" + to_string(i) + ".bin");
		}
		for (int i = 8, j = 0; i <= 15; ++i, ++j) {
			devices[i] = new IODisk("data/DiskUnit-" + to_string(j) + ".bin");
		}
		// Char Devices
		devices[16] = new IOCardReader("data/Cards/In/");
		devices[17] = new IOCardWriter("data/Cards/Out/");
		devices[18] = new IOLinePrinter(cout);
		devices[19] = new IOTypeWriter(cin);
		devices[20] = new IOPaperTape("data/PaperTape.txt");

		INSTR::setC(memory[instrPtr], (uint) OP::SPECIAL);
		INSTR::setF(memory[instrPtr], (uint) FIELD_SPECIAL::HLT);

		while (instrPtr >= 0 && instrPtr < MEM_SIZE) {
			const vWord &instr = memory[instrPtr];
			if (eval(instr)) break;
		}
	};

	// Destructor
	~Machine() {
		for (int i = 0; i < NUM_DEVICES; ++i) {
			delete devices[i];
		}
	};

	// Evaluate the op at instrPtr
	// Returns true if instruction was HLT
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

				switch ((FIELD_SPECIAL)F) {
					case FIELD_SPECIAL::HLT: {
						DEBUG(INFO, "HALT");
						return true;
					}
					case FIELD_SPECIAL::NUM: {
						DEBUG(INFO, "NUM");
						
					}
					case FIELD_SPECIAL::CHAR: {
						DEBUG(INFO, "CHAR");
						
					}
				};
				break;
			}

			// Move
			case OP::MOVE: {
				break;
			}

			// Arithmetic
			case OP::LDA: {
				break;
			}
		};

		instrPtr.data++;
		instrPtr = getLower(instrPtr);
		return false;
	};
};