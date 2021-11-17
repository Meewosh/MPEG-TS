#include "tsCommon.h"
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <windows.h>
#include "tsTransportStream.h"
#include "xTS_AdaptationField.cpp"
#include "xPES_PacketHeader.cpp"
using namespace std;

int main()
{
    xTS  TS;
    // TODO - open file
    int8_t TS_PacketBuffer[TS.TS_PacketLength];
    ifstream file("example_new.ts", ios::binary | ios::in);

    FILE* Fonia;
    errno_t err = fopen_s(&Fonia, "PID136.mp2", "wb");

    if (file.good() == true) {

        cout << "ok" << endl;

        xTS_PacketHeader TS_PacketHeader;
        xTS_AdaptationField TS_AdaptationField;
        xPES_PacketHeader PES_PacketHeader;
        const int PID136 = 136;
        int CC = 0;
        int PES_ovL = 0;

        int32_t TS_PacketId = 0;
        while (!file.eof()) {

            file.read((char*)TS_PacketBuffer, TS.TS_PacketLength);


            TS_PacketHeader.Parse((uint8_t*)TS_PacketBuffer);
            TS_AdaptationField.Parse((uint8_t*)TS_PacketBuffer, (int)TS_PacketHeader.getAfc());

           // printf("%010d ", TS_PacketId);
           //TS_PacketHeader.Print();
           //if (TS_PacketHeader.hasAdaptationField()) TS_AdaptationField.Print();

            if (TS_PacketHeader.getPid() == PID136) {

                if (TS_PacketHeader.isS() == 1) {
                    //cout << " Started";
                    if (TS_PacketHeader.hasPayload()) {
                        for (int i = 4, j = 0; i < 188; i++, j++) {
                            PES_PacketHeader.packet[j] = TS_PacketBuffer[i];
                        }
                    }
                    else if (TS_PacketHeader.hasAdaptationField()) {
                        for (int i = 5 + TS_PacketBuffer[4], j = 0; i < 188 - TS_PacketBuffer[4]; i++, j++) {
                            PES_PacketHeader.packet[j] = TS_PacketBuffer[i];
                        }
                    }

                    PES_PacketHeader.Parse();
                    //PES_PacketHeader.Print();

                    if (TS_PacketHeader.hasAdaptationField()) {
                        int temp = 5 + PES_PacketHeader.getPESHDL() + 6 + 3 + (int)TS_AdaptationField.getL();
                        int temp_m = 188 - 5 - PES_PacketHeader.getPESHDL() - 6 - 3 - (int)TS_AdaptationField.getL();
                        fwrite(TS_PacketBuffer + temp, temp_m, 1, Fonia);
                    }
                    //PES_ovL = PES_PacketHeader.getPESHDL() + PES_PacketHeader.getLEN();

                    CC++;

                }
                else if (TS_PacketHeader.isS() == 0 && TS_PacketHeader.hasAdaptationField()) {

                    //cout << " Finished";

                    if (TS_PacketHeader.hasAdaptationField()) {
                        for (int i = 5 + TS_PacketBuffer[4], j = 0; i < 188 - TS_PacketBuffer[4]; i++, j++) {
                            PES_PacketHeader.packet[j] = TS_PacketBuffer[i];
                        }
                    }

                    cout.width(4);
                    //cout << right << " PES: Len=" << PES_ovL;

                    if (TS_PacketHeader.getAfc() == 3) {

                        fwrite(TS_PacketBuffer + 5 + TS_AdaptationField.getL(), 188 - 5 - TS_AdaptationField.getL(), 1, Fonia);
                        if (TS_PacketHeader.getCc() == 15) CC = 0;
                        if (TS_PacketHeader.getCc() == 1) CC = 2;

                    }
                    else if (TS_PacketHeader.getAfc() == 1) { fwrite(TS_PacketBuffer + 4, 184, 1, Fonia); }

                }
                else if ((CC == TS_PacketHeader.getCc() && TS_PacketHeader.getAfc() == 1) ||
                    TS_PacketHeader.getCc() == 15 && TS_PacketHeader.getAfc() == 1 || TS_PacketHeader.getCc() && TS_PacketHeader.getAfc() == 1) {

                    //cout << " Continue";
                    if (TS_PacketHeader.getAfc() == 3) {
                        fwrite(TS_PacketBuffer + 5 + TS_AdaptationField.getL(), 183, 1, Fonia);
                    }
                    else if (TS_PacketHeader.getAfc() == 1) {
                        fwrite(TS_PacketBuffer + 4, 184, 1, Fonia);
                    }
                    CC++;

                }
                else if (CC < TS_PacketHeader.getCc() || CC  > TS_PacketHeader.getCc()) {

                    //cout << " Packet lost";
                    if (TS_PacketHeader.getAfc() == 0b01) {
                        fwrite(TS_PacketBuffer + 4, 184, 1, Fonia);
                    }

                    CC = 0;

                }



                
            }
           // cout << endl;

            TS_PacketId++;
            //Sleep(350);
        }

    }
    fclose(Fonia);
    Sleep(200);
}