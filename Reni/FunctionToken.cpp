#include "Import.h"
#include "FunctionToken.h"

using namespace Reni;
static bool Trace = true;

class FunctionSyntax final : public Syntax{
    typedef Syntax baseType; 
    typedef FunctionSyntax thisType;
    Ref<Syntax, true> const left;
    Ref<Syntax, true> const right;
public:
    FunctionSyntax(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right)
        :baseType(part)
        , left(left)
        , right(right){
        SetDumpString();
    }
private:
    p_function(Array<String>, DumpData)override {
        return{
            nd(left),
            nd(right)
        };
    }

};


Ref<Syntax> const FunctionToken::CreateSyntax(Ref<Syntax, true> const left, SourcePart const& part, Ref<Syntax, true> const right) const{
    return new FunctionSyntax(left, part, right);
}