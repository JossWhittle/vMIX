#pragma once 

// vMIX Includes
#include "Mem.hpp"

enum class CHAR_INDEX {
	SPACE,
	A, B, C, D, E, F, G, H, I, DELTA, 
	J, K, L, M, N, O, P, Q, R, SIGMA, 
	PI, S, T, U, V, W, X, Y, Z,
	D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
	PERIOD, COMMA, LPAREN, RPAREN, 
	PLUS, SUB, MUL, DIV, EQUALS, DOLLAR, 
	LARROW, RARROW, AT, SEMICOLON, COLON, APOS, NULLCHAR
};

constexpr uint CHARSET_SIZE = 57;
constexpr char CHAR_TABLE[CHARSET_SIZE] = {
	' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'd',
	'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 's', 'p', 
	'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', 
	'3', '4', '5', '6', '7', '8', '9', '.', ',', '(', ')', 
	'+', '-', '*', '/', '=', '$', '<', '>', '@', ';', ':', 
	'\'', ' '
};

constexpr inline char toChar(const uint val) {
	return CHAR_TABLE[val];
};

inline uint toVal(const char code) {
	for (int i = 0; i < CHARSET_SIZE; ++i) {
		if (code == CHAR_TABLE[i]) return i;
	}
	return (uint) CHAR_INDEX::NULLCHAR;
};