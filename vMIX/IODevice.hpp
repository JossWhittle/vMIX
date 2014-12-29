#pragma once

// std Include
#include <fstream>

// vMIX Includes
#include "Mem.hpp"

class IODevice {
private:
	// Members
	uint blockSize;	
	vWord *buffer;

public:
	// Constructor
	IODevice(const uint _blockSize) : blockSize(_blockSize) {
		buffer = new vWord[blockSize]{};
	};
	// Destructor
	virtual ~IODevice() {
		if (buffer != nullptr) delete [] buffer;
	};

	// Interface
	virtual bool in(vWord *addr) = 0;
	virtual bool out(const vWord *addr) = 0;
};

class IOTape : public IODevice {
private:
	// Members
	constexpr static uint NUM_BLOCKS = 2000;

public:
	// Constructor
	IOTape(const string &path) : IODevice(100) {};

	// Interface
	bool in(vWord *addr) {
		return true;
	};
	bool out(const vWord *addr) {
		return true;	
	};
};

class IODisk : public IODevice {
private:
	// Members
	constexpr static uint NUM_BLOCKS = 4000;

public:
	// Constructor
	IODisk(const string &path) : IODevice(100) {};

	// Interface
	bool in(vWord *addr) {
		return true;
	};
	bool out(const vWord *addr) {
		return true;
	};
};

class IOCardReader : public IODevice {
private:
	// Members
	const string dir;
	uint counter = 0;

public:
	// Constructor
	IOCardReader(const string &path) : IODevice(16), dir(path) {};

	// Interface
	bool in(vWord *addr) {
		return true;
	};
	bool out(const vWord *addr) {
		return false;
	};
};

class IOCardWriter : public IODevice {
private:
	// Members
	const string dir;
	uint counter = 0;

public:
	// Constructor
	IOCardWriter(const string &path) : IODevice(16), dir(path) {};

	// Interface
	bool in(vWord *addr) {
		return false;
	};
	bool out(const vWord *addr) {
		return true;
	};
};

class IOLinePrinter : public IODevice {
private:
	// Members
	ostream out;

public:
	// Constructor
	IOLinePrinter(ostream &_out) : IODevice(24), out(_out.rdbuf()) {};

	// Interface
	bool in(vWord *addr) {
		return false;
	};
	bool out(const vWord *addr) {
		return true;
	};
};

class IOTypeWriter : public IODevice {
private:
	// Members
	istream in;

public:
	// Constructor
	IOTypeWriter(istream &_in) : IODevice(14), in(_in.rdbuf()) {};

	// Interface
	bool in(vWord *addr) {
		return true;
	};
	bool out(const vWord *addr) {
		return false;
	};
};

class IOLinePrinter : public IODevice {
private:
	// Members
	ostream out;

public:
	// Constructor
	IOLinePrinter(ostream &_out) : IODevice(24), out(_out.rdbuf()) {};

	// Interface
	bool in(vWord *addr) {
		return false;
	};
	bool out(const vWord *addr) {
		return true;
	};
};

class IOPaperTape : public IODevice {
private:
	// Members
	constexpr static uint NUM_BLOCKS = 1000;

public:
	// Constructor
	IOPaperTape(const string &path) : IODevice(14) {};

	// Interface
	bool in(vWord *addr) {
		return false;
	};
	bool out(const vWord *addr) {
		return true;
	};
};