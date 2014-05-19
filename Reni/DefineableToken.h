#pragma once
#include "TokenClass.h"
#include "../HWLib/String.h"
#include "SearchResult.h"

using namespace HWLib;

namespace Reni
{
    class NumberType;
    class TypeType;

    class DefineableToken 
        : public TokenClass
        , public SearchTarget
    {
        typedef TokenClass baseType;
        typedef DefineableToken thisType;
    public:
        String const name;
    protected:
        DefineableToken(String const&name)
            : name(name)
        {
        };

    public:

        SearchResult const Search(NumberType const&) const override;
        SearchResult const Search(TypeType const&) const override;
        virtual_p(int, priority) { return 0; };
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override;;

        p_function(Array<String>, DumpData) override
        {
            auto result = name.Quote;
            return{result};
        };

        p_function(String, DumpShort) override{return Dump;};
    };


    template<class TNameProvider>
    class NamedDefinableToken : public DefineableToken
    {
        using baseType = DefineableToken; 
        using thisType = NamedDefinableToken;
    protected:
        NamedDefinableToken(): DefineableToken(TNameProvider::Text()) {SetDumpString();}
    };
}