#pragma once

// std Includes
#include <cinttypes>
#include <ostream>
#include <string>

using namespace std;

// Helper
typedef uint32_t uint;
typedef int64_t int64;

// Masks
constexpr uint P = 0;
constexpr uint N = 1;
constexpr uint INDEX_ADDR_MASK = (1 << 12) - 1;
constexpr uint BYTE_MASK	   = (1 << 6)  - 1;

// Machine word
// Five (6 bit) bytes + sign
// Fits into 31 bits
struct vWord {
	// Members
	uint sign:1;
	uint data:30;

	// Overloaded cast to int
	inline operator int() const { return (sign == P) ? data : -data; };
};

// Getters
inline vWord getLower(const vWord &word) {
	return vWord{ word.sign, (word.data & INDEX_ADDR_MASK) };
};

// Pretty Print vWord as Table
inline ostream& operator<<(ostream &os, const vWord &obj) {
	os << string(50, '-') << '\n';
	os << "| " << ((obj.sign == P) ? '+' : '-') << " |";
	for (int i = 4; i >= 0; --i) {
		os << " ";
		for (int j = 5; j >= 0; --j) {
			os << ((obj.data >> ((6 * i) + j)) & 1);
		}
		os << " |";
	}
	os << '\n' << string(50, '-') << '\n';
	return os;
};