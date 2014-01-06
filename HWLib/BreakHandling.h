#pragma once

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

    class Break
    {
    public:
        Break();
        void SelfRegister();
        virtual bool Display(const String&, const String&, int, const String&)const;
        static Break const&This();
    };


    class CAssertionException
    {
    public:
        CAssertionException()
        {
            __asm_int3;
        }
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
#  	define assertx(p,q) if(!(p) && Break::This().Display("Assertion failed: " #p, __FILE__, __LINE__, q )) breakpoint; __analysis_assume(p)
#  	define cbpx(p,q) if((p) && Break::This().Display("Breakpoint: " #p, __FILE__, __LINE__, q )) breakpoint
#  	define bpx(q) if(Break::This().Display("Breakpoint", __FILE__, __LINE__, q )) breakpoint
#	define breakpoint {bool Throw=false; __asm_int3; if(Throw) throw String("breakpoint"); ::HWLib::Console.Write("continued\n");}
#   define returnassert(p) if(p)return ""; else return #p
#endif
#define errbp {bool Throw=true; __asm_int3; if(Throw) throw String("rerrbp"); ::HWLib::Console.Write("continued\n");}

#define assert(p) assertx(p,"")
#define assert_failx(q) assertx(false,q)
#define assert_fail assertx(false,"")
#define cbp(p) cbpx(p,"")
#define bp bpx("")

#define xassertx(p,q) if(!(p) && Break::This().Display("Assertion failed: " #p, __FILE__, __LINE__, q )){ breakpoint; throw CAssertionException();}
#define xassert(p) xassertx(p,"")
#define xassert_failx(q) {xassertx(false,q); throw CAssertionException();}
#define xassert_fail xassertx(false,"")



