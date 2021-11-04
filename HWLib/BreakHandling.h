#pragma once

#include "Console.h"

#ifdef _MANAGED
#   using <mscorlib.dll>
#   define b_core System::Diagnostics::Debugger::Break()
#else
#include <windows.h>
#   define b_core {if (::IsDebuggerPresent())__debugbreak();}

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
#  	define a_if(p,q) {if(!(p) && c_.BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, std::string(q) )) b_throwOpt; __analysis_assume(p);}
#  	define b_if(p,q) {if((p) && c_.BreakTrace("Breakpoint: " #p, __FILE__, __LINE__, std::string(q) )) b_throwOpt;}
#	define b_throwOpt {bool Throw=false; b_core; if(Throw) throw BreakpointException(); c_.Write("continued\n");}
#   define a_return(p) if(p)return ""; else return #p
#endif

#define a_if_(p) a_if(p,)
#define a_fail(q) a_if(false,q)
#define a_fail_ a_if(false,)
#define b_if_(p) b_if(p,)
#define b_ b_if(true,)

#define a_throw(p,q) if(!(p) && c_.BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, std::string(q) )){ b_core; throw AssertionException();}
#define a_throw_(p) a_throw(p,)
#define a_fail_throw(q) a_throw(false,q)
#define a_fail_throw_ a_throw(false,)

#define errorabort(t) return (t)null

#define a_is(left,comparer,right) a_if((left) comparer (right), ("\nleft: "+ nd(left) + "\nright: " + nd(right))|HWLib::Indent() )