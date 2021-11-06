#pragma once

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


#define HW_D_VALUE(x) (std::string(#x) + " = " + HWLib::Dump(x) + ";")
#define nd_arg(i,x) {if(Trace)HW_CONSOLE.Write("[" STRING(i) "] " + HW_D_VALUE(x) + "\n");}
#define d(x) {if(Trace)HW_CONSOLE.Write(HW_D_VALUE(x) + "\n");}
#define dd(x) {if(Trace)HW_CONSOLE.Write(x + "\n");}

#define FILE_LINE_FUNCTION HWLib::String::FilePosition(__FILE__,__LINE__,0,__FUNCTION__)
#define d_here {if(Trace) HW_CONSOLE.Write(FILE_LINE_FUNCTION);}
#define writeLineFilePosition  HW_CONSOLE.WriteLine(FILE_LINE_FUNCTION)
#define srcargdump(x) {if(Trace)HW_CONSOLE.Write(HW_D_VALUE(x)+"\n").fsrcprint();}
#define dumpreturn(x) {\
if(Trace) \
    HW_CONSOLE.Write(std::string("return ") + HW_D_VALUE(x) + "\n"); \
return; }

#define return_d(x) {if(Trace) \
    HW_CONSOLE.Write(std::string("return ") + HW_D_VALUE(x) + "\n"); \
    return x; }

#define return_db(x) {if(Trace) \
        {\
    HW_CONSOLE.Write(std::string("return ") + HW_D_VALUE(x) + "\n");b_}; \
    return x; }

#define HWLib_ARGDUMP(r, data, i, xx) nd_arg(i,xx)
#define HWLib_HEADERDUMP(thisDump) {\
    auto HWLib_HEADERDUMP_indentLevel = HW_CONSOLE.IndentLevel;\
    if(Trace) {\
        HW_CONSOLE.Write(FILE_LINE_FUNCTION);\
        HW_CONSOLE.IndentCount++; \
        HW_CONSOLE.Write("\n");\
        thisDump;\
    };\
}

#define md_ HWLib_HEADERDUMP(HW_CONSOLE.Write(HW_D_VALUE(*this) + "\n"))
#define fd_ HWLib_HEADERDUMP(void())
#define md(...) {HWLib_HEADERDUMP(HW_CONSOLE.Write(HW_D_VALUE(*this) + "\n"));BOOST_PP_SEQ_FOR_EACH_I(HWLib_ARGDUMP, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define fd(...) {HWLib_HEADERDUMP(void());BOOST_PP_SEQ_FOR_EACH_I(HWLib_ARGDUMP, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define md_throw(...) {bool Trace=true;md(__VA_ARGS__);b_;throw AssertionException();}
#define fd_throw(...) {bool Trace=true;fd(__VA_ARGS__);b_;throw AssertionException();}
#define no_fd_
#define no_md_
#define no_fd(...)
#define no_md(...)
#define mb b_;throw BreakpointException()
