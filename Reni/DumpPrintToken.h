#pragma once
#include "DefineableToken.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;

namespace Reni
{
    class AccessType;
    class NumberType;
    class TypeType;

    struct  DumpPrintToken final : public NamedDefinableToken<DumpPrintToken>
    {
        static String const Text(){ return "dump_print"; }
        tc_Search(NumberType);
    };

    struct MinusToken final : public NamedDefinableToken<MinusToken>
    {
        static String const Text(){ return "-"; }
    private:
        tc_Search(NumberType);
        p_function(int, priority) override{ return -4; }
    };
    
    struct PlusToken final : public NamedDefinableToken<PlusToken>
    {
        static String const Text(){ return "+"; }
    private:
        tc_Search(NumberType);
        p_function(int, priority) override{ return -4; }
    };
    
    struct StarToken final : public NamedDefinableToken<StarToken>
    {
        static String const Text(){ return "*"; }
    private:
        tc_Search(NumberType);
        p_function(int, priority) override{ return -2; }
    };

    struct EnableCutToken final : public NamedDefinableToken<EnableCutToken>
    {
        static String const Text(){ return "enable_cut"; }
    private:
        tc_Search(NumberType);
    };

    struct InstanceToken final : public NamedDefinableToken<InstanceToken>
    {
        static String const Text(){ return "instance"; }
    private:
        tc_Search(TypeType);
    };

    struct EqualToken final : public NamedDefinableToken<EqualToken>
    {
        static String const Text(){ return "="; }
    private:
        tc_Search(NumberType);
    };

    struct GreaterToken final : public NamedDefinableToken<GreaterToken>
    {
        static String const Text(){ return ">"; }
    private:
        tc_Search(NumberType);
    };

    struct ColonEqual final : public NamedDefinableToken<ColonEqual>
    {
        static String const Text(){ return ":="; }
    private:
        tc_Search(AccessType);
    };
}
