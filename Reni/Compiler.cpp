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

p_implementation(Compiler, CodeBase, code){
    return _internal->codeCache.Value;
};

p_implementation(Compiler, Ref<CodeItem>, main){
    return _internal->codeCache.Value.main;
};

p_implementation(Compiler, String, cppCode){
    return _internal->cppCode;
};

ExecutionResult const Compiler::Execute(){
    return _internal->Execute();
}

Ref<Syntax> const Compiler::GetSyntaxFromText(String const& text){
    return internal::GetSyntaxFromText(text);
};


p_implementation(CodeBase, String, cppMain)
{
    CodeVisitor visitor;
    a_if(main->exts.isEmpty, nd(main));
    return main->ToCpp(visitor);
};

p_implementation(CodeBase, String, cppFunctions)
{
    return functions
        .Select<String>([&](Global::Function const&function){return function.cppCode; })
        ->Aggregate<String>();

    CodeVisitor visitor;
    a_if(main->exts.isEmpty, nd(main));
    return main->ToCpp(visitor);
};

String const CodeBase::CodeVisitor::Const(Size const size, BitsConst const& value) const
{
    a_if_(size <= BitCount<int>());
    return "return " + String::Convert(int(value)) + ";";
}

