#include "Import.h"
#include "Compiler.h"

#include "BitType.h"
#include "Result.h"
#include "TokenClass.h"
#include "../HWLib/RefCountContainer.instance.h"

static bool Trace = true;
#include "Compiler.internal.h"

using namespace Reni;

Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

p_implementation(Compiler, Ref<Syntax>, syntax){
    return _internal->syntaxCache.Value;
};

p_implementation(Compiler, Ref<CodeItem>, code){
    return _internal->codeCache.Value;
};

p_implementation(Compiler, String, cppCode){
    return _internal->cppCodeCache.Value;
};

ExecutionResult const Compiler::Execute(){
    return _internal->Execute();
}

Ref<Syntax> const Compiler::GetSyntaxFromText(String const& text){
    return internal::GetSyntaxFromText(text);
};


String const Compiler::internal::CodeVisitor::Const(Size const size, BitsConst const& value) const
{
    a_if_(size <= BitCount<int>());
    return "return " + String::Convert(int(value)) + ";";
}

