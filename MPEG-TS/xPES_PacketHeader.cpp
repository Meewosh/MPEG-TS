#include <iostream>
#include <string>
#include <cstdio>
#include <cstdint>
#include <Windows.h>
#include <iostream>
#include "tsCommon.h"


class xPES_PacketHeader {
protected:
	uint32_t PSCP[3];
	uint8_t SID;
	uint16_t LEN;
	uint8_t PESHDL;

public:
	uint8_t packet[188];
	void Parse();
	void Print() const;

public:
	
	uint8_t getSID();
	uint16_t getLEN();
	uint8_t getPESHDL();
};

void xPES_PacketHeader::Parse()
{
	PSCP[0] = packet[0];
	PSCP[1] = packet[1];
	PSCP[2] = packet[2];

	SID = packet[3];
	LEN = xSwapBytes16(*(uint16_t*)&packet[4]);
	PESHDL = packet[8];
}

void xPES_PacketHeader::Print() const
{
	if (PSCP[0] == 0 && PSCP[1] == 0 && PSCP[2] == 1)
	{
		std::cout << " PES: ";
		std::cout << "PSCP=" << PSCP[0] + PSCP[1] + PSCP[2];
		std::cout.width(3);
		std::cout << " SID=";
		std::cout << std::right << int(SID);
		std::cout << " L=";
		std::cout.width(4);
		std::cout << std::right << int(LEN);
	}
}


uint8_t xPES_PacketHeader::getSID()
{
	return SID;
}

uint16_t xPES_PacketHeader::getLEN()
{
	return LEN;
}

uint8_t xPES_PacketHeader::getPESHDL()
{
	return PESHDL;
}
