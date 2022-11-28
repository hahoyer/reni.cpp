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

  class DebugException : public std::exception
  {};

  class BreakpointException : public DebugException
  {};

  class AssertionException : public DebugException
  {};
}

#undef HW_ASSERT_

#ifdef NDEBUG
#  	define HW_ASSERT(p,q) __analysis_assume(p)
#  	define HW_BREAK_IF(p,q) (p)
#	define HW_BREAK_WITH_OPTION_TO_THROW
#else
#  	define HW_ASSERT(p,q) {if(!(p) && HW_CONSOLE.BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, (q) )) HW_BREAK_WITH_OPTION_TO_THROW; __analysis_assume(p);}
#  	define HW_BREAK_IF(p,q) {if((p) && HW_CONSOLE.BreakTrace("Breakpoint: " #p, __FILE__, __LINE__, (q) )) HW_BREAK_WITH_OPTION_TO_THROW;}
#	define HW_BREAK_WITH_OPTION_TO_THROW {bool Throw=false; HW_SYS_BREAKPOINT; if(Throw) throw BreakpointException(); HW_CONSOLE.Write("continued\n");}
#endif

#define HW_ASSERT_(p) HW_ASSERT(p,"")
#define HW_FAIL(q) HW_ASSERT(false,q)
#define HW_FAIL_ HW_ASSERT(false,"")
#define HW_BREAK_IF_(p) HW_BREAK_IF(p,"")
#define HW_BREAKPOINT HW_BREAK_IF(true,"")

#define HW_ASSERT_AND_THROW(p,q) if(!(p) && HW_CONSOLE.BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, std::string(q) )){ HW_SYS_BREAKPOINT; throw AssertionException();}
#define HW_ASSERT_AND_THROW_(p) HW_ASSERT_AND_THROW(p,"")
#define HW_FAIL_AND_THROW(q) HW_ASSERT_AND_THROW(false,q)
#define HW_FAIL_AND_THROW_ HW_ASSERT_AND_THROW(false,"")

#define HW_ASSERT_IS(left,comparer,right) HW_ASSERT((left) comparer (right), ("\nleft: "+ HW_D_VALUE(left) + "\nright: " + HW_D_VALUE(right))|HWLib::Indent() )
