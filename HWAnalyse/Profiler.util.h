#pragma once

#pragma warning (disable : 4201) // C4201 nonstandard extension used : nameless struct/union
#include "windows.h"
#pragma warning (default : 4201) // C4201 nonstandard extension used : nameless struct/union
#include "../HWLib/System.h"
using namespace  HWLib;


void dumpprint(const char*Text){
    ::OutputDebugString(Text);
};

size_t dumpprint(__int64 x){
    char Text[100];
    _ltoa_s(int(x), Text, sizeof(Text), 10);
    dumpprint(Text);
    return ::strlen(Text);
};

void dumpprint(__int64 x, int Scale, const char*xText){
    char Text[100];
    _ltoa_s(int(x), Text + 1, sizeof(Text)-1, 10);

    int Dot = 1;
    for(; Text[Dot]; Dot++);
    Dot -= Scale;
    Dot--;
    for(int i = 0; i < Dot; i++)
        Text[i] = Text[i + 1];
    if(Scale > 0) Text[Dot] = '.';
    else Text[Dot] = 0;
    dumpprint(Text);
    dumpprint(xText);
};


void dumpprinttime(__int64 x, __int64 q){
    double xx = double(x) / (double(System::TicksPerSecond())*double(q)*3600.0);
    if(xx > 10.0){
        dumpprint(int(xx), 0, "h");
        return;
    };

    if(xx >= 1.0){
        int h = int(xx);
        dumpprint(h);
        dumpprint(":");
        xx -= h;
        xx *= 60;
        int m = int(xx);
        if(m<10) dumpprint("0");
        dumpprint(m);
        return;
    };

    xx *= 60;
    if(xx >= 1.0){
        dumpprint(":");
        int m = int(xx);
        if(m<10) dumpprint("0");
        dumpprint(m);
        dumpprint(":");
        xx -= m;
        xx *= 60;
        int s = int(xx);
        if(s<10) dumpprint("0");
        dumpprint(s);
        return;
    };

    xx *= 600;

    char* Unit[6] = {"sec", "msec", "µsec", "nsec", "psec", "fsec"};
    for(int i = 1; i<18; i++){
        if(xx >= 100.0){
            dumpprint(int(xx), i % 3, Unit[i / 3]);
            return;
        };
        xx *= 10;
    };
    dumpprint("0");
};


