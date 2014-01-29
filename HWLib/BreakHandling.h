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

    class CAssertionException
    {
    public:
        CAssertionException(){__asm_int3;}
    };
}

#undef assert

#ifdef NDEBUG
#  	define assertx(p,q) __analysis_assume(p)
#  	define cbpx(p,q) (p)
#  	define bpx(q)
#	define breakpoint
#   define returnassert(p) return ""
#else
#  	define assertx(p,q) if(!(p) && _console_ BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, q )) breakpoint; __analysis_assume(p)
#  	define cbpx(p,q) if((p) && _console_ BreakTrace("Breakpoint: " #p, __FILE__, __LINE__, q )) breakpoint
#  	define bpx(q) if(_console_ BreakTrace("Breakpoint", __FILE__, __LINE__, q )) breakpoint
#	define breakpoint {bool Throw=false; __asm_int3; if(Throw) throw String("breakpoint"); _console_ Write("continued\n");}
#   define returnassert(p) if(p)return ""; else return #p
#endif
#define errbp {bool Throw=true; __asm_int3; if(Throw) throw String("rerrbp"); _console_ Write("continued\n");}

#define assert(p) assertx(p,"")
#define assert_failx(q) assertx(false,q)
#define assert_fail assertx(false,"")
#define cbp(p) cbpx(p,"")
#define bp bpx("")

#define xassertx(p,q) if(!(p) && _console_ BreakTrace("Assertion failed: " #p, __FILE__, __LINE__, q )){ breakpoint; throw CAssertionException();}
#define xassert(p) xassertx(p,"")
#define xassert_failx(q) {xassertx(false,q); throw CAssertionException();}
#define xassert_fail xassertx(false,"")

#define errorabort(t) return (t)null

