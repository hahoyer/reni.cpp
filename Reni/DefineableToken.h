#pragma once
#include "TokenClass.h"
#include "SearchTarget.h"

#include "../HWLib/string.h"

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
        string const name;
    protected:
        DefineableToken(string const&name)
            : name(name)
        {
        };

    public:

        SearchResult<Feature> const Declarations(NumberType const&) const override;
        SearchResult<Feature> const Declarations(TypeType const&) const override;
        SearchResult<Feature> const Declarations(EnableCutType const&) const override;
        SearchResult<Feature> const Declarations(AccessType const&) const override;
        HW_PR_VIRTUAL_GET(int, priority) { return 0; };
    private:
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override;;

        HW_PR_DECL_GETTER(Array<string>, DumpData) override
        {
            auto result = name|Quote();
            return{result};
        };

        HW_PR_DECL_GETTER(string, DumpShort) override{return Dump;};
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

