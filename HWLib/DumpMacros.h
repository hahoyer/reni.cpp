#pragma once

//#define vardumpf(x,d) (String(#x) + "=" + ::dump(x,d) + ";")
#define vardump(x) (HWLib::String(#x) + " = " + HWLib::DumpToString(x) + ";")
//#define varxdump(x) (String(#x) + "=0x" + String(x,16) + ";")
//#define structdump(x) (String(#x ":\n").print(),(x).dumpprint(),String("\n").print())

#define src HWLib::String::FilePosition(__FILE__,__LINE__,0,__FUNCTION__)
#define srcprint() _fsrcprint(__FILE__,__LINE__,0,__FUNCTION__)
#define srcprintln() _fsrcprintln(__FILE__,__LINE__,0,__FUNCTION__)
#define dumpo if(Trace) _console_ FunctionTrace
#define argdump(x) {if(Trace)_console_ Write(vardump(x)+"\n");}
#define argxdump(x) {if(Trace)_console_ Write(varxdump(x)+"\n");}
#define srcargdump(x) {if(Trace)_console_ (vardump(x)+"\n").fsrcprint();}
#define srcargxdump(x) {if(Trace)_console_ (varxdump(x)+"\n").fsrcprint();}
#define dumpreturn(x) {if(Trace) \
    _console_ IndentLevel--; \
    _console_ Write(HWLib::String("return ") + vardump(x) + "\n"); \
    return; }

#define returndump(x) {if(Trace) \
    _console_ IndentLevel--; \
    _console_ Write(HWLib::String("return ") + vardump(x) + "\n"); \
    return x; }

#define returndumpb(x) {if(Trace) \
    {bpx(HWLib::String("\1-return ") + vardump(x) + "\n"); }; \
    return x; }

#define mdump()       dumpo(__FUNCTION__,*this)
#define mdumpc(a)      dumpo(__FUNCTION__,*this); argdump(a)
#define mdumpc2(a,b)    dumpo(__FUNCTION__,*this); argdump(a); argdump(b)
#define mdumpc3(a,b,c)   dumpo(__FUNCTION__,*this); argdump(a); argdump(b); argdump(c)
#define mdumpc4(a,b,c,d)  dumpo(__FUNCTION__,*this); argdump(a); argdump(b); argdump(c); argdump(d)
#define mdumpc5(a,b,c,d,e) dumpo(__FUNCTION__,*this); argdump(a); argdump(b); argdump(c); argdump(d); argdump(e)

#define ni_mdump()       bool Trace=true;mdump();bp;throw CAssertionException()
#define ni_mdumpc(a)      bool Trace=true;mdumpc(a);bp;throw CAssertionException()
#define ni_mdumpc2(a,b)    bool Trace=true;mdumpc2(a,b);bp;throw CAssertionException()
#define ni_mdumpc3(a,b,c)   bool Trace=true;mdumpc3(a,b,c);bp;throw CAssertionException()
#define ni_mdumpc4(a,b,c,d)  bool Trace=true;mdumpc4(a,b,c,d);bp;throw CAssertionException()
#define ni_mdumpc5(a,b,c,d,e) bool Trace=true;mdumpc5(a,b,c,d,e);bp;throw CAssertionException()

#define fdump()           if(Trace) FunctionTrace(__FUNCTION__)
#define fdumpc(a)        fdump(); argdump(a);
#define fdumpc2(a,b)    fdump(); argdump(a); argdump(b);
#define fdumpc3(a,b,c)   fdump(); argdump(a); argdump(b); argdump(c);
#define fdumpc4(a,b,c,d)  fdump(); argdump(a); argdump(b); argdump(c); argdump(d); 
#define fdumpc5(a,b,c,d,e) fdump(); argdump(a); argdump(b); argdump(c); argdump(d); argdump(e); 

#define ni_fdump()       bool Trace=true;fdump();bp;throw CAssertionException()
#define ni_fdumpc(a)      bool Trace=true;fdumpc(a);bp;throw CAssertionException()
#define ni_fdumpc2(a,b)    bool Trace=true;fdumpc2(a,b);bp;throw CAssertionException()
#define ni_fdumpc3(a,b,c)   bool Trace=true;fdumpc3(a,b,c);bp;throw CAssertionException()
#define ni_fdumpc4(a,b,c,d)  bool Trace=true;fdumpc4(a,b,c,d);bp;throw CAssertionException()
#define ni_fdumpc5(a,b,c,d,e) bool Trace=true;fdumpc5(a,b,c,d,e);bp;throw CAssertionException()

#define no_mdump()            bool Trace=false; dumpo(__FUNCTION__,*this)
#define no_mdumpc(a)          no_mdump()
#define no_mdumpc2(a,b)       no_mdump()
#define no_mdumpc3(a,b,c)     no_mdump()
#define no_mdumpc4(a,b,c,d)   no_mdump()
#define no_mdumpc5(a,b,c,d,e) no_mdump()
