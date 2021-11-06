#pragma once

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


#define HW_D_VALUE(x) (std::string(#x) + " = " + HWLib::Dump(x) + ";")
#define HW_D_VALUE_I(i,x) {if(Trace)HW_CONSOLE.Write("[" STRING(i) "] " + HW_D_VALUE(x) + "\n");}
#define HW_D_LOG_VALUE(x) {if(Trace)HW_CONSOLE.Write(HW_D_VALUE(x) + "\n");}
#define HW_D_LOG(x) {if(Trace)HW_CONSOLE.Write((x) + "\n");}

#define HW_FILE_LINE_FUNCTION HWLib::String::FilePosition(__FILE__,__LINE__,0,__FUNCTION__)
#define HW_D_LOG_HERE {if(Trace) HW_CONSOLE.Write(HW_FILE_LINE_FUNCTION);}
#define HW_WRITE_LINE_FILE_POSITION  HW_CONSOLE.WriteLine(HW_FILE_LINE_FUNCTION)

#define return_d(x) {if(Trace) \
    HW_CONSOLE.Write(std::string("return ") + HW_D_VALUE(x) + "\n"); \
    return x; }

#define return_db(x) {if(Trace) \
        {\
    HW_CONSOLE.Write(std::string("return ") + HW_D_VALUE(x) + "\n");HW_BREAKPOINT}; \
    return x; }

#define HW_D_LOG_HEADER(thisDump) {\
    auto HW_D_LOG_HEADER_outerIndentLevel = HW_CONSOLE.IndentLevel;\
    if(Trace) {\
        HW_CONSOLE.Write(HW_FILE_LINE_FUNCTION);\
        HW_CONSOLE.IndentCount++; \
        HW_CONSOLE.Write("\n");\
        thisDump;\
    };\
}

#define HW_D_ARG_FOR_BOOST(r, data, i, xx) HW_D_VALUE_I(i,xx)
#define HW_D_METHOD_ HW_D_LOG_HEADER(HW_CONSOLE.Write(HW_D_VALUE(*this) + "\n"))
#define HW_D_FUNCTION_ HW_D_LOG_HEADER(void())
#define HW_D_METHOD(...) {HW_D_LOG_HEADER(HW_CONSOLE.Write(HW_D_VALUE(*this) + "\n"));BOOST_PP_SEQ_FOR_EACH_I(HW_D_ARG_FOR_BOOST, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define HW_D_FUNCTION(...) {HW_D_LOG_HEADER(void());BOOST_PP_SEQ_FOR_EACH_I(HW_D_ARG_FOR_BOOST, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));}
#define HW_D_METHOD_THROW(...) {bool Trace=true;HW_D_METHOD(__VA_ARGS__);HW_BREAKPOINT;throw AssertionException();}
#define HW_D_FUNCTION_THROW(...) {bool Trace=true;HW_D_FUNCTION(__VA_ARGS__);HW_BREAKPOINT;throw AssertionException();}
#define NO_HW_D_FUNCTION_
#define NO_HW_D_METHOD_
#define NO_HW_D_FUNCTION(...)
#define NO_HW_D_METHOD(...)
#define HW_BREAK_AND_THROW HW_BREAKPOINT;throw BreakpointException()
