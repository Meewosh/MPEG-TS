#pragma once
#include "tsCommon.h"
#include <string>
#include <cstdio>
#include <cstdint>
#include <Windows.h>
#include <iostream>

/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS {
public:
    static constexpr uint32_t TS_PacketLength = 188;
    static constexpr uint32_t TS_HeaderLength = 4;

    static constexpr uint32_t PES_HeaderLength = 6;

    static constexpr uint32_t BaseClockFrequency_Hz = 90000; //Hz
    static constexpr uint32_t ExtendedClockFrequency_Hz = 27000000; //Hz
    static constexpr uint32_t BaseClockFrequency_kHz = 90; //kHz
    static constexpr uint32_t ExtendedClockFrequency_kHz = 27000; //kHz
    static constexpr uint32_t BaseToExtendedClockMultiplier = 300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{
public:
    enum class ePID : uint16_t
    {
        PAT = 0x0000,
        CAT = 0x0001,
        TSDT = 0x0002,
        IPMT = 0x0003,
        NIT = 0x0010, //DVB specific PID
        SDT = 0x0011, //DVB specific PID
        NuLL = 0x1FFF,
    };

protected:
    //TODO - header fields
    uint8_t SB;
    bool E;
    bool S;
    bool T;
    uint16_t PID;
    uint8_t TSC;
    int AFC;
    int CC;
public:

    void  Parse(uint8_t* Input);
    void  Print() const;

public:

    uint8_t getSb() const;

    bool isE() const;

    bool isS() const;

    bool isT() const;

    uint16_t getPid() const;

    uint8_t getTsc() const;

    uint8_t getAfc() const;

    int getCc() const;

public:

    bool     hasAdaptationField();
    bool     hasPayload();

};

//=============================================================================================================================================================================

void xTS_PacketHeader::Parse(uint8_t* Input) {
    uint32_t HDR = xSwapBytes32(*(uint32_t*)Input);
    SB = (HDR & 0xFF000000) >> 24;
    E = (HDR & 0x00800000) >> 23;
    S = (HDR & 0x00400000) >> 22;
    T = (HDR & 0x00200000) >> 21;
    PID = (HDR & 0x001FFF00) >> 8;
    TSC = (HDR & 0x000000C0) >> 6;
    AFC = (HDR & 0x00000030) >> 4;
    CC = (HDR & 0x0000000F);
}

uint8_t xTS_PacketHeader::getSb() const {
    return SB;
}

bool xTS_PacketHeader::isE() const {
    return E;
}

bool xTS_PacketHeader::isS() const {
    return S;
}

bool xTS_PacketHeader::isT() const {
    return T;
}

uint16_t xTS_PacketHeader::getPid() const {
    return PID;
}

uint8_t xTS_PacketHeader::getTsc() const {
    return TSC;
}

uint8_t xTS_PacketHeader::getAfc() const {
    return AFC;
}

int xTS_PacketHeader::getCc() const {
    return CC;
}

inline bool xTS_PacketHeader::hasAdaptationField()
{
    if (AFC == 2 or AFC == 3) return true;
    else return false;
}

inline bool xTS_PacketHeader::hasPayload()
{
    if (AFC == 1) return true;
    else return false;
}

void xTS_PacketHeader::Print() const {
    
    int counter = 0;
    //show_counter(counter);
    counter++;
    std::cout << " TS: ";

    std::cout.width(3);
    std::cout << "SB=" << (int)getSb();

    std::cout << " E=" << (int)isE();

    std::cout << " S=" << (int)isS();

    std::cout << " T=" << (int)isT();

    std::cout << " PID=";
    std::cout.width(4);
    std::cout << std::right << (int)getPid();

    std::cout << " TSC=" << (int)getTsc();
    
    std::cout << " AFC=" << (int)getAfc();
    
    std::cout << " CC=";
    std::cout.width(2);
    std::cout << std::right << (int)getCc();
}


