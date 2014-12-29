#pragma once

// vMIX Includes
#include "Mem.hpp"
#include "OpCode.hpp"

namespace INSTR {
	// Packed Instruction Size
	constexpr uint INSTR_AA_SIZE   = 12;
	constexpr uint INSTR_I_SIZE    = 6;
	constexpr uint INSTR_F_SIZE    = 6;
	constexpr uint INSTR_C_SIZE    = 6;

	// Packed Instruction Shifts
	constexpr uint INSTR_AA_SHIFT  = 18;
	constexpr uint INSTR_I_SHIFT   = 12;
	constexpr uint INSTR_F_SHIFT   = 6;

	// Packed Instruction Masks
	constexpr uint INSTR_AA_MASK   = ((1 << INSTR_AA_SIZE) - 1) << INSTR_AA_SHIFT;
	constexpr uint INSTR_I_MASK    = ((1 << INSTR_I_SIZE)  - 1) << INSTR_I_SHIFT;
	constexpr uint INSTR_F_MASK    = ((1 << INSTR_F_SIZE)  - 1) << INSTR_F_SHIFT;
	constexpr uint INSTR_C_MASK    = ((1 << INSTR_C_SIZE)  - 1);

	// Setters
	inline void setAA(vWord &word, uint val) {
		word.data = (word.data & ~INSTR_AA_MASK) | ((val & ~(1 << INSTR_AA_SIZE)) << INSTR_AA_SHIFT);
	};
	inline void setI(vWord &word, uint val) {
		word.data = (word.data & ~INSTR_I_MASK) | ((val & ~(1 << INSTR_I_SIZE)) << INSTR_I_SHIFT);
	};
	inline void setF(vWord &word, uint val) {
		word.data = (word.data & ~INSTR_F_MASK) | ((val & ~(1 << INSTR_F_SIZE)) << INSTR_F_SHIFT);
	};
	inline void setC(vWord &word, uint val) {
		word.data = (word.data & ~INSTR_C_MASK) | (val & ~(1 << INSTR_C_SIZE));
	};

	// Getters
	inline uint getAA(const vWord &word) {
		return (word.data & INSTR_AA_MASK) >> INSTR_AA_SHIFT;
	};
	inline uint getI(const vWord &word) {
		return (word.data & INSTR_I_MASK) >> INSTR_I_SHIFT;
	};
	inline uint getF(const vWord &word) {
		return (word.data & INSTR_F_MASK) >> INSTR_F_SHIFT;
	};
	inline uint getC(const vWord &word) {
		return (word.data & INSTR_C_MASK);
	};
};

namespace FIELD {
	// Calculate L:R from F
	inline void calcLR(const uint f, uint &L, uint &R) {
		R = f % 8;
		L = (f - R) / 8;
	};
	// Calculate size of sub-field from F
	inline uint calcSize(const uint f) {
		uint L, R;
		calcLR(f, L, R);
		return (R - L) + 1;
	};
	
	// Getters
	inline vWord get(const vWord &word, const uint f) {
		// Special case for default 0:5 field
		if (f == 5) return word;

		// Calculate the field
		uint L, R;
		calcLR(f, L, R);
		uint s = (R - L) + 1;

		// Return is positive unless L == 0
		vWord ret{P};
		
		// Copy sign
		if (L == 0) {
			ret.sign = word.sign;
			--s;
		}

		// If no data bytes to copy
		if (R == 0) return ret;

		// Extract sub-field
		const uint msk = (1 << (s * 6)) - 1;
		if (R == 5) {
			// No shifting needed
			ret.data = (word.data) & msk;
		}
		else {
			const uint sft = ((5 - R) * 6);
			ret.data = (word.data >> sft) & msk;
		}

		return ret;
	};
};