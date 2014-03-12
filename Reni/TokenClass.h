#pragma once
#include "../HWLang/SourcePart.h"
#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLang;
using namespace HWLib;

namespace Reni
{
    class FeatureClass;
    class SearchResult;
    class Syntax;
    class Type;

    class TokenClass 
        : public WithId<DumpableObject, TokenClass>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, TokenClass>;
        using thisType = TokenClass;
    public:
        TokenClass() = default;
        TokenClass(TokenClass const&) = delete;

        ref_p;
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