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

        AssumeConstObject;

        virtual bool AcceptsMatch(bool isMatch)const{ return !isMatch; };
        virtual_p(Optional<bool>, HasLeft){ return{}; }
        virtual_p(Optional<bool>, HasRight){ return{}; }

        Ref<Syntax> const Mismatch(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const;

        virtual Ref<Syntax> const Infix   (Ref<Syntax >const left, SourcePart const&part, Ref<Syntax      >const right)const;
        virtual Ref<Syntax> const Suffix  (Ref<Syntax >const left, SourcePart const&part)const;
        virtual Ref<Syntax> const Sufanyfix(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax, true>const right)const;

        virtual Ref<Syntax> const Prefix(SourcePart const&part, Ref<Syntax      >const right)const;
        virtual Ref<Syntax> const Terminal(SourcePart const&part)const;
        virtual Ref<Syntax> const NoSuffix(SourcePart const&part, Ref<Syntax, true>const right)const;

        virtual Ref<Syntax> const Preanyfix(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax    >const right)const;
        virtual Ref<Syntax> const NoPrefix (Ref<Syntax, true>const left, SourcePart const&part)const;
        virtual Ref<Syntax> const Anyfix(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const;

        virtual_p(WeakRef<FeatureClass>, featureClass) = 0;
    private:
        p_function(Array<String>,DumpData) override{
            return{};
        };
    };
};