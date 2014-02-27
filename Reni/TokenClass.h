#pragma once
#include "..\HWLib\DumpableObject.h"
#include "..\HWLib\Ref.h"
#include "..\HWLang\SourcePart.h"

namespace Reni
{
    class Syntax;

    class TokenClass : public WithId<DumpableObject, TokenClass>{
        using baseType = WithId<DumpableObject, TokenClass>;
        using thisType = TokenClass;
    public:
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax >const right, bool isMatch)const;
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part, bool isMatch)const;
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax >const right, bool isMatch)const;
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, bool isMatch)const;
    private:
        override_p_function(Array<String>, DumpData){
            return{};
        };
    };
};