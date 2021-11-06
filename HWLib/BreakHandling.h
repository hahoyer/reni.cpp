#pragma once

#include "Console.h"

#ifdef _MANAGED
#   using <mscorlib.dll>
#   define HW_SYS_BREAKPOINT System::Diagnostics::Debugger::Break()
#else
#include <windows.h>
#   define HW_SYS_BREAKPOINT {if (::IsDebuggerPresent())__debugbreak();}

#endif

namespace HWLib
{
    class Console;

    class DebugException: public std::exception{};
    class BreakpointException : public DebugException{};
    class AssertionException : public DebugException{};
}

#undef a_if_

#ifdef NDEBUG
#  	define a_if(p,q) __analysis_assume(p)
#  	define b_if(p,q) (p)
#	define b_throwOpt
#   define a_return(p) return ""
#else
#  	define a_if(p,q) {if(!(p) && HW_CONSOLE.BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, std::string(q) )) b_throwOpt; __analysis_assume(p);}
#  	define b_if(p,q) {if((p) && HW_CONSOLE.BreakTrace("Breakpoint: " #p, __FILE__, __LINE__, std::string(q) )) b_throwOpt;}
#	define b_throwOpt {bool Throw=false; HW_SYS_BREAKPOINT; if(Throw) throw BreakpointException(); HW_CONSOLE.Write("continued\n");}
#   define a_return(p) if(p)return ""; else return #p
#endif

#define a_if_(p) a_if(p,)
#define a_fail(q) a_if(false,q)
#define a_fail_ a_if(false,)
#define b_if_(p) b_if(p,)
#define HW_BREAKPOINT b_if(true,)

#define a_throw(p,q) if(!(p) && HW_CONSOLE.BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, std::string(q) )){ HW_SYS_BREAKPOINT; throw AssertionException();}
#define a_throw_(p) a_throw(p,)
#define a_fail_throw(q) a_throw(false,q)
#define a_fail_throw_ a_throw(false,)

#define errorabort(t) return (t)null

#define a_is(left,comparer,right) a_if((left) comparer (right), ("\nleft: "+ HW_D_VALUE(left) + "\nright: " + HW_D_VALUE(right))|HWLib::Indent() )