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
	IODevice *devices[NUM_DEVICES]{};

	// Registers
	vWord rA{}, rX{}, rI1{}, rI2{}, rI3{}, rI4{}, rI5{}, rI6{}, rJ{};

	// Internal RAM
	vWord memory[MEM_SIZE]{};

	// CPU Frequency
	uint  tik;
	bool HALT;
	vWord instrPtr;

	// Helpers
	inline void sleep(const uint t) const {
		this_thread::sleep_for(chrono::milliseconds(t * tik));
	};

	inline void setInstruction(vWord &ptr, const OP c, const uint aa, const uint i, const uint f) {
		INSTR::setC(memory[ptr], (uint) c);
		INSTR::setAA(memory[ptr], aa);
		INSTR::setI(memory[ptr], i);
		INSTR::setF(memory[ptr], f);
		cout << "[INSTRSET] : Addr: " 
			 << to_string(ptr) << '\n' 
			 << memory[ptr] << '\n';
		ptr.data++;
		ptr = getLower(ptr);
	};
	inline void setMem(vWord &ptr, int val) {
		if (val < 0) {
			memory[ptr] = vWord{ N, -val };
		}
		else {
			memory[ptr] = vWord{ P, val };
		}
		cout << "[ MEMSET ] : Addr: " 
			 << to_string(ptr) << '\n' 
			 << memory[ptr] << '\n';
		ptr.data++;
		ptr = getLower(ptr);
	};

public:
	// Default Constructor
	Machine(const uint _tik, const vWord _entry) : tik{_tik}, instrPtr{_entry}, HALT{false} {

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


		vWord ptr = instrPtr;
		vWord memPtr = vWord{ P, 1000 };
		for (int i = 0; i < 6; ++i) {
			setMem(memPtr, -i);
		}
		setInstruction(ptr, OP::LDA, 1000, 0, FIELD::calcF(1,5));
		setInstruction(ptr, OP::ADD, 1001, 0, FIELD::calcF(1,5));
		setInstruction(ptr, OP::ADD, 1002, 0, FIELD::calcF(1,5));
		setInstruction(ptr, OP::ADD, 1003, 0, FIELD::calcF(0,5));
		setInstruction(ptr, OP::ADD, 1004, 0, FIELD::calcF(0,5));
		setInstruction(ptr, OP::ADD, 1005, 0, FIELD::calcF(0,5));
		setInstruction(ptr, OP::STA, 1006, 0, FIELD::calcF(0,2));
		setInstruction(ptr, OP::OUT, 1000, 0, 18);
		setInstruction(ptr, OP::IN, 1000, 0, 19);
		setInstruction(ptr, OP::OUT, 1000, 0, 18);
		setInstruction(ptr, OP::SPECIAL, 0, 0, (uint) FIELD_SPECIAL::HLT);

		DEBUG(DEBUG_FLAG::INFO, "Running ...");
		DEBUG(DEBUG_FLAG::INFO, "");

		while (!HALT && instrPtr >= 0 && instrPtr < MEM_SIZE) {
			eval(memory[instrPtr]);
		}
		cout << "rA:\n" << rA;
		
		for (int i = 0; i < 7; ++i) {
			cout << "mem[100" << to_string(i) << "]:\n" << memory[1000 + i];
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
		int AA = INSTR::getAA(instr);
		const uint C  = INSTR::getC(instr);
		const uint F  = INSTR::getF(instr);
		const uint I  = INSTR::getI(instr);

		// Perform Address Indexing
		switch (I) {
			case 1: {
				AA += rI1;
				break;
			}
			case 2: {
				AA += rI2;
				break;
			}
			case 3: {
				AA += rI3;
				break;
			}
			case 4: {
				AA += rI4;
				break;
			}
			case 5: {
				AA += rI5;
				break;
			}
			case 6: {
				AA += rI6;
				break;
			}
		};

		// Check for indexing errors
		if (AA < 0) {
			DEBUG(DEBUG_FLAG::ERROR, "AA ptr Underflow!");
			AA = 0;
		}
		else if (AA > (MEM_SIZE - 1)) {
			DEBUG(DEBUG_FLAG::ERROR, "AA ptr Overflow!");
			AA = (MEM_SIZE - 1);
		}

		// Sleep for base op time
		sleep(OP_TIME[C]);

		switch ((OP)C) {
			case OP::NOP: break;
			
			// Arithmetic
			case OP::ADD: {
				OP_Add(AA, F);
				break;
			}
			case OP::SUB: {
				OP_Sub(AA, F);
				break;
			}
			case OP::MUL: {
				OP_Mul(AA, F);
				break;
			}
			case OP::DIV: {
				OP_Div(AA, F);
				break;
			}

			// Special
			case OP::SPECIAL: {
				OP_Special(AA, F);
				break;
			}

			// Move
			case OP::MOVE: {
				OP_Move(AA, F);
				break;
			}

			// Load
			case OP::LDA: {
				OP_Load(rA, AA, F);
				break;
			}
			case OP::LD1: {
				OP_Load(rI1, AA, F);
				rI1 = getLower(rI1);
				break;
			}
			case OP::LD2: {
				OP_Load(rI2, AA, F);
				rI2 = getLower(rI2);
				break;
			}
			case OP::LD3: {
				OP_Load(rI3, AA, F);
				rI3 = getLower(rI3);
				break;
			}
			case OP::LD4: {
				OP_Load(rI4, AA, F);
				rI4 = getLower(rI4);
				break;
			}
			case OP::LD5: {
				OP_Load(rI5, AA, F);
				rI5 = getLower(rI5);
				break;
			}
			case OP::LD6: {
				OP_Load(rI6, AA, F);
				rI6 = getLower(rI6);
				break;
			}
			case OP::LDX: {
				OP_Load(rX, AA, F);
				break;
			}
			case OP::LDAN: {
				OP_LoadN(rA, AA, F);
				break;
			}
			case OP::LD1N: {
				OP_LoadN(rI1, AA, F);
				rI1 = getLower(rI1);
				break;
			}
			case OP::LD2N: {
				OP_LoadN(rI2, AA, F);
				rI2 = getLower(rI2);
				break;
			}
			case OP::LD3N: {
				OP_LoadN(rI3, AA, F);
				rI3 = getLower(rI3);
				break;
			}
			case OP::LD4N: {
				OP_LoadN(rI4, AA, F);
				rI4 = getLower(rI4);
				break;
			}
			case OP::LD5N: {
				OP_LoadN(rI5, AA, F);
				rI5 = getLower(rI5);
				break;
			}
			case OP::LD6N: {
				OP_LoadN(rI6, AA, F);
				rI6 = getLower(rI6);
				break;
			}
			case OP::LDXN: {
				OP_LoadN(rX, AA, F);
				break;
			}

			// Store
			case OP::STA: {
				OP_Store(rA, AA, F);
				break;
			}
			case OP::ST1: {
				OP_Store(vWord{ P, getLower(rI1).data }, AA, F);
				break;
			}
			case OP::ST2: {
				OP_Store(vWord{ P, getLower(rI2).data }, AA, F);
				break;
			}
			case OP::ST3: {
				OP_Store(vWord{ P, getLower(rI3).data }, AA, F);
				break;
			}
			case OP::ST4: {
				OP_Store(vWord{ P, getLower(rI4).data }, AA, F);
				break;
			}
			case OP::ST5: {
				OP_Store(vWord{ P, getLower(rI5).data }, AA, F);
				break;
			}
			case OP::ST6: {
				OP_Store(vWord{ P, getLower(rI6).data }, AA, F);
				break;
			}
			case OP::STX: {
				OP_Store(rX, AA, F);
				break;
			}
			case OP::STJ: {
				OP_Store(vWord{ P, getLower(rJ).data }, AA, F);
				break;
			}
			case OP::STZ: {
				OP_Store(vWord{}, AA, F);
				break;
			}

			// IO Control
			case OP::JBUS: {
				break;
			}
			case OP::IOC: {
				break;
			}
			case OP::IN: {
				// Wait for device
				while (devices[F]->busy()) sleep(1);
				devices[F]->in(&memory[AA]);
				break;
			}
			case OP::OUT: {
				// Wait for device
				while (devices[F]->busy()) sleep(1);
				devices[F]->out(&memory[AA]);
				break;
			}
			case OP::JRED: {
				break;
			}

			// Jump
			case OP::JUMP: {
				break;
			}
			case OP::JA: {
				break;
			}
			case OP::J1: {
				break;
			}
			case OP::J2: {
				break;
			}
			case OP::J3: {
				break;
			}
			case OP::J4: {
				break;
			}
			case OP::J5: {
				break;
			}
			case OP::J6: {
				break;
			}
			case OP::JX: {
				break;
			}

			// Mod
			case OP::MODA: {
				break;
			}
			case OP::MOD1: {
				break;
			}
			case OP::MOD2: {
				break;
			}
			case OP::MOD3: {
				break;
			}
			case OP::MOD4: {
				break;
			}
			case OP::MOD5: {
				break;
			}
			case OP::MOD6: {
				break;
			}
			case OP::MODX: {
				break;
			}

			// Comparisons
			case OP::CMPA: {
				break;
			}
			case OP::CMP1: {
				break;
			}
			case OP::CMP2: {
				break;
			}
			case OP::CMP3: {
				break;
			}
			case OP::CMP4: {
				break;
			}
			case OP::CMP5: {
				break;
			}
			case OP::CMP6: {
				break;
			}
			case OP::CMPX: {
				break;
			}
		};

		instrPtr.data++;
		instrPtr = getLower(instrPtr);
		return false;
	};

	inline void OP_Special(const uint AA, const uint F) {
		switch ((FIELD_SPECIAL)F) {
			case FIELD_SPECIAL::HLT: {
				DEBUG(INFO, "HALT");
				HALT = true;
				break;
			}
			case FIELD_SPECIAL::NUM: {
				DEBUG(INFO, "NUM");	
				break;
			}
			case FIELD_SPECIAL::CHAR: {
				DEBUG(INFO, "CHAR");
				break;
			}
		};
	};

	inline void OP_Add(const uint AA, const uint F) {
		const vWord &src = FIELD::get(memory[AA], F);
		OP_Add(rA, src);
	};
	inline void OP_Add(vWord &dst, const vWord &src) {
		const int tmp = dst + src;
		if (tmp < 0) {
			dst = vWord{ N, (uint) (-tmp) };
		}
		else {
			dst = vWord{ P, (uint) (tmp) };
		}
	};
	inline void OP_HAdd(vWord &dst, const vWord &src) {
		OP_Add(dst, src);
		dst = getLower(dst);
	};
	inline void OP_Sub(const uint AA, const uint F) {
		const vWord &src = FIELD::get(memory[AA], F);
		OP_Sub(rA, src);
	};
	inline void OP_Sub(vWord &dst, const vWord &src) {
		const int tmp = dst - src;
		if (tmp < 0) {
			dst = vWord{ N, (uint) (-tmp) };
		}
		else {
			dst = vWord{ P, (uint) (tmp) };
		}
	};
	inline void OP_HSub(vWord &dst, const vWord &src) {
		OP_Sub(dst, src);
		dst = getLower(dst);
	};
	inline void OP_Mul(const uint AA, const uint F) {
		const vWord &src = FIELD::get(memory[AA], F);
		const int64 tmp = (int64) rA * (int64) src;
		if (tmp < 0) {
			rA = vWord{ N, (uint) (-tmp) << 30 };
			rX = vWord{ N, (uint) (-tmp) };
		}
		else {
			rA = vWord{ P, (uint) (tmp) << 30 };
			rX = vWord{ P, (uint) (tmp) };
		}
	};
	inline void OP_Div(const uint AA, const uint F) {
		const vWord &src = FIELD::get(memory[AA], F);
		const int64 ax = ((int64) rA << 30) | (int64) rX;
		const int64 tmp = ax / (int64) src;
		if (tmp < 0) {
			rA = vWord{ N, (uint) (-tmp) };
		}
		else {
			rA = vWord{ P, (uint) (tmp) };
		}
	};
	inline void OP_Move(const uint AA, const uint F) {
		vWord *M = &memory[AA];
		for (int i = 0; i < F; ++i) {
			memory[rI1] = *M++;
			OP_HAdd(rI1, vWord{ P, 1 });
			sleep(1);
		}
	};
	inline void OP_Load(vWord &reg, const uint AA, const uint F) {
		FIELD::set(reg, F, memory[AA]);
	};
	inline void OP_LoadN(vWord &reg, const uint AA, const uint F) {
		FIELD::set(reg, F, memory[AA]);
		reg.sign = (reg < 0) ? P : N; // Flip sign
	};
	inline void OP_Store(vWord &reg, const uint AA, const uint F) {
		// Special case for default 0:5 field
		if (F == 5) {
			memory[AA] = reg;
			return;
		}

		// Calculate the field
		uint L, R;
		FIELD::calcLR(F, L, R);
		uint s = (R - L) + 1;

		// Return is positive unless L == 0
		vWord ret{ P };

		// Copy sign
		if (L == 0) {
			ret.sign = reg.sign;
			--s;
		}

		// If no data bytes to copy
		if (R == 0) return;

		// Extract sub-field
		const uint msk = (1 << (s * 6)) - 1;
		if (R == 5) {
			// No shifting needed
			ret.data = reg.data | (ret.data & ~msk);
		}
		else {
			const uint sft = ((5 - R) * 6);
			ret.data = ((reg.data & msk) << sft) | (ret.data & (~msk << sft));
		}

		memory[AA] = ret;
	};
};