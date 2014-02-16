#pragma once

#include "Console.h"

#ifdef _MANAGED
#   using <mscorlib.dll>
#   define __asm_int3 System::Diagnostics::Debugger::Break()
#else
#   define __asm_int3 __asm{ int 3 }
#endif

namespace HWLib
{
    class String;
    class Console;

    class AssertionException
    {
    public:
        AssertionException(){__asm_int3;}
    };
}

#undef a_if_

#ifdef NDEBUG
#  	define a_if(p,q) __analysis_assume(p)
#  	define b_if(p,q) (p)
#  	define b(q)
#	define breakpoint
#   define a_return(p) return ""
#else
#  	define a_if(p,q) if(!(p) && _console_ BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, String(q) )) breakpoint; __analysis_assume(p)
#  	define b_if(p,q) if((p) && _console_ BreakTrace("Breakpoint: " #p, __FILE__, __LINE__, String(q) )) breakpoint
#  	define b(q) if(_console_ BreakTrace("Breakpoint", __FILE__, __LINE__, String(q) )) breakpoint
#	define breakpoint {bool Throw=false; __asm_int3; if(Throw) throw String("breakpoint"); _console_ Write("continued\n");}
#   define a_return(p) if(p)return ""; else return #p
#endif
#define errbp {bool Throw=true; __asm_int3; if(Throw) throw String("rerrbp"); _console_ Write("continued\n");}

#define a_if_(p) a_if(p,)
#define a_fail(q) a_if(false,q)
#define a_fail_ a_if(false,)
#define b_if_(p) b_if(p,)
#define b_ b("")

#define a_throw(p,q) if(!(p) && _console_ BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, String(q) )){ breakpoint; throw AssertionException();}
#define a_throw_(p) a_throw(p,)
#define a_fail_throw(q) {a_throw(false,q); throw AssertionException();}
#define a_fail_throw_ a_throw(false,)

#define errorabort(t) return (t)null

#define a_is(left,comparer,right) a_if((left)comparer(right), ("\nleft: "+ nd(left) + "\nright: " + nd(right)).Indent() )