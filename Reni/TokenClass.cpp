#include "Import.h"
#include "TokenClass.h"
#include "Syntax.h"

using namespace Reni;
static bool Trace = true;


Ref<Syntax const> const TokenClass::Create(Ref<Syntax const>const left, SourcePart part, Ref<Syntax const>const right)const
{
    mdumpc3(left, part, right);
    returndump(Ref<Syntax const>(new Syntax));
};

Ref<Syntax const> const TokenClass::Create(Ref<Syntax const>const left, SourcePart part)const{
    mdumpc2(left, part);
    returndump(Ref<Syntax const>(new Syntax));
};

Ref<Syntax const> const TokenClass::Create(SourcePart part, Ref<Syntax const>const right)const{
    mdumpc2(part, right);
    returndump(Ref<Syntax const>(new Syntax));
};

Ref<Syntax const> const TokenClass::Create(SourcePart part)const
{
    mdumpc(part);
    returndump(Ref<Syntax const>(new Syntax));
}

p_implementation(TokenClass, Array<String>, DumpData){
    return List<String>{};
};

