#pragma once

/* *************************************************************************

Source Profiler -- Copyright (C) 1995 - 2021 - Harald Hoyer
-----------------------------------------------------

Strategy:

With macros HW_PROF_BEGIN and HW_PROF_END you define regions of source code that
build an entry of profiler table. At run time for such a region each call is
counted and time elapsed is accumulated.
All data is stored in a global variable called Profile.
With member function dump the results can be printed into Microsoft dump
context at any time.
Time measurement is done by use of best timer available (1.12 MHz, about 900 nano seconds)

---

Usage:

Include:	Profiler.H
Include this file in each source file you want to monitor and wherein
you want to put the Profile.dump call.

Source:		Profiler.CPP
Include this file in your project list or make file.

---

Functions / Macros:

HW_PROF_BEGIN( <FlagText> );

Defines a start point of a region to monitor. profiler uses file name and line
number as identification for accumulation. The parameter FlagText is an
additional string that will be also presented in profile dump and serves as
a comment. If at run time a region is already active, its measurements are
stopped and the values are saved.


HW_PROF_END();

Defines the ending point of a region to monitor. Each HW_PROF_END must have
a corresponding HW_PROF_BEGIN call. Association is done dynamically. After HW_PROF_END
the former region is resumed, if there is one.


HW_PROF_NEXT( <FlagText> );

Ends up the current region and starts a new one.


HW_PROF_FUNCTION(expression)

Creates a profile region that surround the evaluation of expression. The expression itself
is used as parameter "flagtext".


HW_PROF_STATEMENT(expression)

Same as HW_PROF_FUNCTION, use it for void-type expressions.


Profile.dump(int Hide = 10);

Dump accumulated values into MFCs dump context. The output format is:

<FileName>(<LineNr>): [<SeqNo>]:	<CallCount>	<TimePerCall>	<ElapsedTime>	<FlagText>

For each HW_PROF_BEGIN one line such line is generated. The lines are sorted by
elapsed time. With MSVC you can use the F4-key ("Goto Error/Tag") on these lines.
The parameter Hide is used to restrict the amount of lines. If it is negative
it determines the lines to print (-1 -> first line only, -2 -> first two
lines and so on). If it is zero no restriction applies. If Hide is positive
the last lines that all together consume elapsed time of not more than 1/Hide
of all are hidden (1 -> no output, 2 -> one half is shown, 3 -> 2/3 is shown
10 -> 90% is shown).


Profile.reset();

Clear counters of all regions and retain the memory used. This function may
only be used when no region is active.


#define HW_PROF_INSTANCE <Instance of CHWProfile>

Use this define before include to enable measurement and
to provide the profile variable to use. The thing provided here should be a pointer
to CHWProfile


#undef HW_PROF_INSTANCE

Use this before include to disable measurement (this is the default).

To avoid confusion, use #define / #undef of HW_PROF_INSTANCE only in CPP-Files and never
in Headers.

************************************************************************* */

namespace HWAnalysis
{
  class Profiler final
  {
    class internal;
    internal& _internal;

  public:
    Profiler();
    ~Profiler();

    Profiler(const Profiler&) = delete;
    Profiler(Profiler&&) = delete;
    Profiler& operator=(const Profiler&) = delete;
    Profiler& operator=(Profiler&&) = delete;

    void Start(const char* FileName, int Line, const char* Flag) const;
    void End() const;
    void dump(int Hide = 10) const;
    void Reset() const;

    template <class T>
    static T FunctionReturn(T result);
  };
}

#ifndef HW_PROF_INSTANCE
#	define HW_PROF_BEGIN(flag)
#	define HW_PROF_FUNCTION(expression) (expression)
#	define HW_PROF_STATEMENT(expression) (expression)
#	define HW_PROF_NEXT(flag)
#	define HW_PROF_END()

template <class T>
T HWAnalysis::Profiler::FunctionReturn(T result) { return result; };

#else
#	define HW_PROF_BEGIN(flag) if(HW_PROF_INSTANCE) (HW_PROF_INSTANCE)->Start(__FILE__, __LINE__,flag )
#	define HW_PROF_FUNCTION(expression) ((HW_PROF_INSTANCE)->Start(__FILE__, __LINE__,#expression ), HWAnalysis::Profiler::FunctionReturn(expression))
#	define HW_PROF_STATEMENT(expression) ((HW_PROF_INSTANCE)->Start(__FILE__, __LINE__,#expression ), expression, (HW_PROF_INSTANCE)->End())
#	define HW_PROF_END() if(HW_PROF_INSTANCE) (HW_PROF_INSTANCE)->End()
#	define HW_PROF_NEXT(flag) HW_PROF_END();HW_PROF_BEGIN(flag)
template<class T>
T HWAnalysis::Profiler::FunctionReturn(T result)
{
    (HW_PROF_INSTANCE)->End();
    return result;
};
#endif
