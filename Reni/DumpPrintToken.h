#pragma once
#include "DefineableToken.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;

#define tc_Search(targetType)SearchResult const Search(targetType const& target) const override
#define tc_Search_implementation(tokenClass,targetType)SearchResult const tokenClass::Search(targetType const& target) const{return target.Search<tokenClass>();}

namespace Reni
{
    class NumberType;
    class TypeType;

    struct  DumpPrintToken final : public NamedDefinableToken<DumpPrintToken>
    {
        static String const Text(){ return "dump_print"; }
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
}
