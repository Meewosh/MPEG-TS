#include <iostream>
#include <string>
#include <cstdio>
#include <cstdint>
#include <Windows.h>
#include <iostream>
#include "tsCommon.h"

using namespace std;

class xTS_AdaptationField {
protected:
	uint8_t L;
	bool DC;
	bool RA;
	bool SP1;
	bool PR;
	bool OR;
	bool SP2;
	bool TP;
	bool EX;
public:
	//void Reset();
	void Parse(const uint8_t* Input, uint8_t AdaptationFieldControl);
	void Print() const;
public:
	uint8_t getL();
	bool getDC();
	bool getRA();
	bool getSP1();
	bool getPR();
	bool getOR();
	bool getSP2();
	bool getTP();
	bool getEX();
	uint32_t getNumBytes() const { };
};

void xTS_AdaptationField::Parse(const uint8_t* Input, uint8_t AdaptationFieldControl) {
	if (AdaptationFieldControl == 2 || AdaptationFieldControl == 3) {
		uint64_t AF = xSwapBytes64((*(uint64_t*)Input));
		L = (AF & 0x00000000FF000000) >> 24;
		DC = (AF & 0x0000000000800000) >> 23;
		RA = (AF & 0x0000000000400000) >> 22;
		SP1 = (AF & 0x0000000000200000) >> 21;
		PR = (AF & 0x0000000000100000) >> 20;
		OR = (AF & 0x0000000000080000) >> 19;
		SP2 = (AF & 0x0000000000040000) >> 18;
		TP = (AF & 0x0000000000020000) >> 17;
		EX = (AF & 0x0000000000010000) >> 16;
	}
}



uint8_t xTS_AdaptationField::getL()
{
	return L;
}

bool xTS_AdaptationField::getDC()
{
	return DC;
}

bool xTS_AdaptationField::getRA()
{
	return RA;
}

bool xTS_AdaptationField::getSP1()
{
	return SP1;
}

bool xTS_AdaptationField::getPR()
{
	return PR;
}

bool xTS_AdaptationField::getOR()
{
	return OR;
}

bool xTS_AdaptationField::getSP2()
{
	return SP2;
}

bool xTS_AdaptationField::getTP()
{
	return TP;
}

bool xTS_AdaptationField::getEX()
{
	return EX;
}

void xTS_AdaptationField::Print() const {
	
	cout << " AF: ";
	cout << "L=";
	cout.width(3);
	cout << (int)L;
	cout << " DC=" << (int)DC;
	cout << " RA=" << (int)RA;
	cout << " SP=" << (int)SP1;
	cout << " PR=" << (int)PR;
	cout << " OR=" << (int)OR;
	cout << " SP=" << (int)SP2;
	cout << " TP=" << (int)TP;
	cout << " EX=" << (int)EX;
	
}