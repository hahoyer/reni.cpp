#pragma once
#include "../HWLang/SourcePart.h"
#include "../HWLib/Ref.h"

using namespace HWLang;
using namespace HWLib;

namespace Reni
{
    class FeatureClass;
    class SearchResult;
    class Syntax;
    class Type;

    class TokenClass : public WithId<DumpableObject, TokenClass>{
        using baseType = WithId<DumpableObject, TokenClass>;
        using thisType = TokenClass;
    public:
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax >const right, bool isMatch)const;
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part, bool isMatch)const;
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax >const right, bool isMatch)const;
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, bool isMatch)const;
        virtual_p(WeakRef<FeatureClass>, featureClass) = 0;
    private:
        override_p_function(Array<String>, DumpData){
            return{};
        };
    };
};