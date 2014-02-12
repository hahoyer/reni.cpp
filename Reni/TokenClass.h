#pragma once

namespace Reni
{
    class Syntax;

    struct TokenClass: public DumpableObject
    {
        Ref<Syntax const> const Create(Ref<Syntax const>const left, SourcePart part, Ref<Syntax const>const right)const;
        Ref<Syntax const> const Create(Ref<Syntax const>const left, SourcePart part)const;
        Ref<Syntax const> const Create(SourcePart part, Ref<Syntax const>const right)const;
        Ref<Syntax const> const Create(SourcePart part)const;
    private:
        virtual p_function(Array<String>, DumpData)override;
    };
}