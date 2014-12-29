#pragma once

// Op Codes
enum struct OP : uint { 
	NOP,

	// Arithmetic
	ADD,
	SUB,
	MUL,
	DIV,

	// NUM(0), CHAR(1), HLT(2)
	SPECIAL,

	// SLA(0), SRA(1), SLAX(2), SRAX(3), SLC(4), SRC(5)
	SHIFT,

	MOVE,

	// Load 
	LDA,
	LD1, LD2, LD3, LD4, LD5, LD6,
	LDX,
	LDAN,
	LD1N, LD2N, LD3N, LD4N, LD5N, LD6N,
	LDXN,

	// Store
	STA,
	ST1, ST2, ST3, ST4, ST5, ST6,
	STX,
	STJ,
	STZ, // Zero Mem

	// IO Control (0)
	JBUS,
	IOC,
	IN, OUT,
	JRED,

	// JMP(0), JSJ(1), JOV(2), JNOV(3), JL(4), 
	// JE(5), JG(6), JGE(7), JNE(8), JLE(9)
	JUMP,

	// N(0), Z(1), P(2), NN(3), NZ(4), NP(5)
	JA,
	J1, J2, J3, J4, J5, J6,
	JX,

	// INC(0), DEC(1), ENT(2), ENN(3)
	MODA,
	MOD1, MOD2, MOD3, MOD4, MOD5, MOD6,
	MODX,

	// Comparisons
	CMPA,
	CMP1, CMP2, CMP3, CMP4, CMP5, CMP6,
	CMPX,

	// Counter
	NUM_CODES
};

// Relative timings for each base op
constexpr uint OP_TIME[64] = {
	1,  2,  2, 10, 12, 10,  2,  1,
	2,	2,	2,	2,	2,	2,	2,	2,
	2,	2,	2,	2,	2,	2,	2,	2,
	2,	2,	2,	2,	2,	2,	2,	2,
	2,  2,  1,  1,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,
	2,	2,	2,	2,	2,	2,	2,	2
};

// Field Modifiers for instructions
enum struct FIELD_SPECIAL : uint {
	NUM,
	CHAR,
	HLT,

	// Counter
	NUM_CODES
};

enum struct FIELD_SHIFT : uint {
	SLA,
	SRA,
	SLAX,
	SRAX,
	SLC,
	SRC,

	// Counter
	NUM_CODES
};

enum struct FIELD_JUMP : uint {
	JMP,
	JSJ,
	JOV,
	JNOV,
	JL,
	JE,
	JG,
	JGE,
	JNE,
	JLE,

	// Counter
	NUM_CODES
};

enum struct FIELD_J : uint {
	N,
	Z,
	P,
	NN,
	NZ,
	NP,

	// Counter
	NUM_CODES
};

enum struct FIELD_MOD : uint {
	INC,
	DEC,
	ENT,
	ENN,

	// Counter
	NUM_CODES
};