#pragma once
#include "../HWLang/SourcePart.h"
#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Optional.h"

using namespace HWLang;
using namespace HWLib;

namespace Reni
{
    template <class TFeature>
    class SearchResult;
    class Syntax;
    class Type;
    class SyntaxArgVisitor;


    class TokenClass 
        : public WithId<DumpableObject, TokenClass>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, TokenClass>;
        using thisType = TokenClass;
    public:
        TokenClass() = default;
        TokenClass(TokenClass const&) = delete;

        ThisRef;

        virtual Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const = 0;
    private:
        p_function(Array<string>, DumpData) override{ return{}; };
    };

    class TerminalTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef TerminalTokenClass thisType;
    public:
        virtual Ref<Syntax> const Create(SourcePart const&part)const = 0;
        Optional<Ref<Syntax>> Replace(SyntaxArgVisitor const&) const { return {}; };
    private:
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override final{
            a_if(left.IsEmpty, nd(left) + "\n" + nd(*this) + "\n" + nd(part) + "\n" + nd(right));
            a_if(right.IsEmpty, nd(left) + "\n" + nd(*this) + "\n" + nd(part) + "\n" + nd(right));
            return Create(part);
        };
    };


    class PrefixTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef PrefixTokenClass thisType;
    public:
        virtual Ref<Syntax> const Create(SourcePart const&part, Ref<Syntax>const right)const = 0;
    private:
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override final{
            a_if_(left.IsEmpty);
            a_if_(!right.IsEmpty);
            return Create(part, right);
        };
    };


    class SuffixTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef SuffixTokenClass thisType;
    public:
        virtual Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part)const = 0;
    private:
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override final{
            a_if_(!left.IsEmpty);
            a_if_(right.IsEmpty);
            return Create(left, part);
        };
    };


    class InfixTokenClass : public TokenClass{
        typedef TokenClass baseType;
        typedef InfixTokenClass thisType;
    public:
        virtual Ref<Syntax> const Create(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right)const = 0;
    private:
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override final{
            a_if_(!left.IsEmpty);
            a_if_(!right.IsEmpty);
            return Create(left, part, right);
        };
    };
};

