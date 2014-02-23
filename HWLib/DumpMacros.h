#pragma once

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


#define nd(x) (HWLib::String(#x) + " = " + HWLib::Dump(x) + ";")
#define nd_arg(i,x) {if(Trace)_console_ Write("[" STRING(i) "] " + nd(x) + "\n");}
#define d(x) {if(Trace)_console_ Write(nd(x) + "\n");}
#define dd(x) {if(Trace)_console_ Write(x + "\n");}

#define FILE_LINE_FUNCTION HWLib::String::FilePosition(__FILE__,__LINE__,0,__FUNCTION__)
#define d_here {if(Trace) _console_ Write(FILE_LINE_FUNCTION);}
#define writeLineFilePosition  _console_ WriteLine(FILE_LINE_FUNCTION)
#define srcargdump(x) {if(Trace)_console_ Write(nd(x)+"\n").fsrcprint();}
#define dumpreturn(x) {if(Trace) \
    _console_ IndentLevel--; \
    _console_ Write(HWLib::String("return ") + nd(x) + "\n"); \
    return; }

#define return_d(x) {if(Trace) \
    {_console_ IndentLevel--; \
    _console_ Write(HWLib::String("return ") + nd(x) + "\n");}; \
    return x; }

#define return_db(x) {if(Trace) \
    {b(HWLib::String("\1-return ") + nd(x) + "\n"); }; \
    return x; }

#define HWLib_ARGDUMP(r, data, i, xx) nd_arg(i,xx)
#define HWLib_HEADERDUMP(thisDump) {\
    if(Trace) {\
        _console_ Write(FILE_LINE_FUNCTION);\
        _console_ IndentLevel++; \
        _console_ Write("\n");\
        thisDump;\
    };\
}

#define md_ HWLib_HEADERDUMP(_console_ Write(nd(*this) + "\n"))
#define fd_ HWLib_HEADERDUMP(void())
#define md(...) {HWLib_HEADERDUMP(_console_ Write(nd(*this) + "\n"));BOOST_PP_SEQ_FOR_EACH_I(HWLib_ARGDUMP, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define fd(...) {HWLib_HEADERDUMP(void());BOOST_PP_SEQ_FOR_EACH_I(HWLib_ARGDUMP, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define md_throw(...) {bool Trace=true;md(__VA_ARGS__);b_;throw AssertionException();}
#define fd_throw(...) {bool Trace=true;fd(__VA_ARGS__);b_;throw AssertionException();}
#define no_fd_
#define no_md_
#define no_fd(...)
#define no_md(...)
