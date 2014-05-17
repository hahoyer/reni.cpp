#pragma once
#include "TokenClass.h"
#include "ExpressionSyntax.h"
#include "../HWLib/String.h"

using namespace HWLib;

namespace Reni
{
    class DefineableToken : public TokenClass
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
        virtual SearchResult const Search(TypeType const&) const{return{};}
        virtual SearchResult const Search(NumberType const&target) const;
        virtual_p(int, priority) { return 0; };
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override
        {
            return new ExpressionSyntax(*this, left, part, right);
        };

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