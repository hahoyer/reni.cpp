#pragma once
#include "TokenClass.h"
#include "SearchTarget.h"

#include "../HWLib/String.h"

using namespace HWLib;

namespace Reni
{
    class NumberType;
    class TypeType;
    class EnableCutType;

    class DefineableToken 
        : public TokenClass
        , public DeclarationType
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

        SearchResult<Feature> const Declarations(NumberType const&) const override;
        SearchResult<Feature> const Declarations(TypeType const&) const override;
        SearchResult<Feature> const Declarations(EnableCutType const&) const override;
        virtual_p(int, priority) { return 0; };
    private:
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override;;

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

#define tc_Search(targetType) SearchResult<Feature> const Declarations(targetType const& target) const override

#define tc_Search_implementation(tokenClass,targetType) \
SearchResult<Feature> const tokenClass::Declarations(targetType const& target) const\
{\
    return target.DeclarationsForType<tokenClass>();\
}

