#include "Import.h"
#if 0
#include "Token.h"
#include "TokenClass.h"
#include "PrioTable.h"
#include "Syntax.h"

using namespace Reni;

Ref<Syntax const> const Token::Create(OptRef<Syntax const> const left, OptRef<Syntax const> right)const
{
    if (left.IsValid)
        if (right.IsValid)
            return Class.Create(left, Part, right);
        else
            return Class.Create(left, Part);
    else
        if (right.IsValid)
            return Class.Create(Part, right);
        else
            return Class.Create(Part);
};

p_implementation(Token, String, Name)
{
    if (IsEnd)
        return PrioTableConst::EndOfText;
    if (IsStart)
        return PrioTableConst::BeginOfText;
    return Part;

};

p_implementation(Token, Array<String>, DumpData){
    return
    {
        nd(Class),
        nd(Part)
    };
};

p_implementation(Token, String, DumpShort){
    return baseType::get_DumpShort() + "{"+Name + "}";
};
#endif