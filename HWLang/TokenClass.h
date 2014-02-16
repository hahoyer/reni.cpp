#pragma once

namespace HWLang
{
    template<class Syntax>
    class AbstractTokenClass: public DumpableObject
    {
    public:
        virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart part, Ref<Syntax const>const right)const = 0;
        virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart part)const = 0;
        virtual Ref<Syntax const> const CreateSyntax(SourcePart part, Ref<Syntax const>const right)const = 0;
        virtual Ref<Syntax const> const CreateSyntax(SourcePart part)const = 0;
    };
}