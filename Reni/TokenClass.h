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
        Ref<Syntax> const Mismatch(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const;
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const = 0;

        virtual_p(WeakRef<FeatureClass>, featureClass) = 0;
    private:
        p_function(Array<String>,DumpData) override{
            return{};
        };
    };

    class TerminalTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef TerminalTokenClass thisType;
    public:
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part)const = 0;
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override final{
            a_if(left.IsEmpty, nd(left) + "\n" + nd(*this) + "\n" + nd(part) + "\n" + nd(right));
            a_if_(right.IsEmpty);
            return CreateSyntax(part);
        };
    };

    class PrefixTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef PrefixTokenClass thisType;
    public:
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax>const right)const = 0;
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override final{
            a_if_(left.IsEmpty);
            a_if_(!right.IsEmpty);
            return CreateSyntax(part, right);
        };
    };

    class SuffixTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef SuffixTokenClass thisType;
    public:
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax>const left, SourcePart const&part)const = 0;
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override final{
            a_if_(!left.IsEmpty);
            a_if_(right.IsEmpty);
            return CreateSyntax(left, part);
        };
    };
};