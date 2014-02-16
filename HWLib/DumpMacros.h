#pragma once

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


#define nd(x) (HWLib::String(#x) + " = " + HWLib::Dump(x) + ";")
#define nd_arg(i,x) {if(Trace)_console_ Write("[" i "] " + nd(x) + "\n");}
#define d(x) {if(Trace)_console_ Write(nd(x) + "\n");}

#define src HWLib::String::FilePosition(__FILE__,__LINE__,0,__FUNCTION__)
#define writeFilePosition _console_ Write(src)
#define writeLineFilePosition  _console_ WriteLine(src)
#define srcargdump(x) {if(Trace)_console_ Write(nd(x)+"\n").fsrcprint();}
#define dumpreturn(x) {if(Trace) \
    _console_ IndentLevel--; \
    _console_ Write(HWLib::String("return ") + nd(x) + "\n"); \
    return; }

#define returndump(x) {if(Trace) \
    _console_ IndentLevel--; \
    _console_ Write(HWLib::String("return ") + nd(x) + "\n"); \
    return x; }

#define returndumpb(x) {if(Trace) \
    {bb(HWLib::String("\1-return ") + nd(x) + "\n"); }; \
    return x; }

#define HWLib_ARGDUMP(r, data, i, xx) nd_arg(i,xx)
#define HWLib_HEADERDUMP(thisDump, ...) {if(Trace) {_console_ FunctionTrace(__FUNCTION__);thisDump;}; BOOST_PP_SEQ_FOR_EACH_I(HWLib_ARGDUMP, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define HWLib_HEADERDUMP_(thisDump) {if(Trace) {_console_ FunctionTrace(__FUNCTION__);thisDump;}}

#define md_ HWLib_HEADERDUMP_(_console_ Write(nd(*this)))
#define fd_ HWLib_HEADERDUMP_(void())
#define md(...) HWLib_HEADERDUMP(_console_ Write(nd(*this)),__VA_ARGS__)
#define fd(...) HWLib_HEADERDUMP(,__VA_ARGS__)
#define md_throw(...) {bool Trace=true;md(__VA_ARGS__);b_;throw AssertionException();}
#define fd_throw(...) {bool Trace=true;fd(__VA_ARGS__);b_;throw AssertionException();}
#define no_fd_
#define no_md_
#define no_fd(...)
#define no_md(...)
