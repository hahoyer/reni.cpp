#pragma once
#include "DefineableToken.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;

namespace Reni
{
    class NumberType;

#define tt(nameHeader,text) \
    struct  nameHeader##Token final : public NamedDefinableToken<nameHeader##Token>\
    {                                                      \
    static String const Text(){ return #text; }\
    }


    struct  DumpPrintToken final : public NamedDefinableToken<DumpPrintToken>
    {
        static String const Text(){ return "dump_print"; }
    };

    struct MinusToken final : public NamedDefinableToken<MinusToken>
    {
        static String const Text(){ return "-"; }
    private:
        SearchResult const Search(NumberType const& target) const override;
    };
    
    struct PlusToken final : public NamedDefinableToken<PlusToken>
    {
        static String const Text(){ return "+"; }
    private:
        SearchResult const Search(NumberType const& target) const override;
    };
    
    struct StarToken final : public NamedDefinableToken<StarToken>
    {
        static String const Text(){ return "*"; }
    private:
        SearchResult const Search(NumberType const& target) const override;
    };

    struct InstanceToken final : public NamedDefinableToken<InstanceToken>
    {
        static String const Text(){ return "instance"; }
    private:
        SearchResult const Search(TypeType const& target) const override;
    };
}