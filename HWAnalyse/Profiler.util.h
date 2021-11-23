#pragma once

#pragma warning (disable : 4201) // C4201 nonstandard extension used : nameless struct/union
#include "windows.h"
#pragma warning (default : 4201) // C4201 nonstandard extension used : nameless struct/union
#include "../HWLib/System.h"
using namespace HWLib;


inline void dump(const char* Text)
{
  ::OutputDebugString(Text);
};

inline size_t dump(__int64 x)
{
  char Text[100];
  _ltoa_s(static_cast<int>(x), Text, sizeof Text, 10);
  dump(Text);
  return strlen(Text);
};

inline void dump(__int64 x, int Scale, const char* xText)
{
  char Text[100];
  _ltoa_s(static_cast<int>(x), Text + 1, sizeof Text - 1, 10);

  int Dot = 1;
  for(; Text[Dot]; Dot++)continue;
  Dot -= Scale;
  Dot--;
  for(int i = 0; i < Dot; i++)
    Text[i] = Text[i + 1];
  if(Scale > 0) Text[Dot] = '.';
  else Text[Dot] = 0;
  dump(Text);
  dump(xText);
};


inline void dumpTime(__int64 x, __int64 q)
{
  double xx = static_cast<double>(x) / (static_cast<double>(System::TicksPerSecond()) * static_cast<double>(q) *
    3600.0);
  if(xx > 10.0)
  {
    dump(static_cast<int>(xx), 0, "h");
    return;
  };

  if(xx >= 1.0)
  {
    const int h = static_cast<int>(xx);
    dump(h);
    dump(":");
    xx -= h;
    xx *= 60;
    const int m = static_cast<int>(xx);
    if(m < 10) dump("0");
    dump(m);
    return;
  };

  xx *= 60;
  if(xx >= 1.0)
  {
    dump(":");
    const int m = static_cast<int>(xx);
    if(m < 10) dump("0");
    dump(m);
    dump(":");
    xx -= m;
    xx *= 60;
    const int s = static_cast<int>(xx);
    if(s < 10) dump("0");
    dump(s);
    return;
  };

  xx *= 600;

  const char* Unit[6] = {"sec", "msec", "µsec", "nsec", "psec", "fsec"};
  for(int i = 1; i < 18; i++)
  {
    if(xx >= 100.0)
    {
      dump(static_cast<int>(xx), i % 3, Unit[i / 3]);
      return;
    };
    xx *= 10;
  };
  dump("0");
};
