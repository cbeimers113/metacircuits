#include "Circuit.h"

// An extremely hacky way to get a String instance from a std::string of unknown length. Supports strings up to 6 chars in length
String std_string_to_String(std::string inString) {
	int l = inString.length();
	if (l == 1) {
		char cArr[2];
		cArr[0] = inString.at(0);
		cArr[l] = '\0';
		return String(cArr);
	}
	else if (l == 2) {
		char cArr[3];
		for (int i = 0; i < l; i++)
			cArr[i] = inString.at(i);
		cArr[l] = '\0';
		return String(cArr);
	}
	else if (l == 3) {
		char cArr[4];
		for (int i = 0; i < l; i++)
			cArr[i] = inString.at(i);
		cArr[l] = '\0';
		return String(cArr);
	}
	else if (l == 4) {
		char cArr[5];
		for (int i = 0; i < l; i++)
			cArr[i] = inString.at(i);
		cArr[l] = '\0';
		return String(cArr);
	}
	else if (l == 5) {
		char cArr[6];
		for (int i = 0; i < l; i++)
			cArr[i] = inString.at(i);
		cArr[l] = '\0';
		return String(cArr);
	}
	else if (l >= 6) {
		char cArr[7];
		for (int i = 0; i < 6; i++)
			cArr[i] = inString.at(i);
		cArr[l] = '\0';
		return String(cArr);
	}
	return String("ERR");
}