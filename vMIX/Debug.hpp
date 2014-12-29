#pragma once

// std Includes
#include <iostream>
#include <string>

using namespace std;

enum DEBUG_FLAG {
	ERROR,
	WARNING,
	INFO
};

inline void DEBUG(const DEBUG_FLAG lvl, const string &msg) {
	switch (lvl) {
		case ERROR: {
			cout << "[ ERROR ]";
			break;
		}
		case WARNING: {
			cout << "[WARNING]";
			break;
		}
		case INFO: {
			cout << "[  INFO ]";
			break;
		}
	};

	cout << " : " << msg << '\n';
};