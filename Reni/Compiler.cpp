#include "Import.h"
#include "Compiler.h"

#include "BitType.h"
#include "ContainerContext.h"
#include "Result.h"
#include "SyntaxContainer.h"
#include "TokenClass.h"

#include "../HWLib/RefCountContainer.instance.h"

static bool Trace = true;
#include "Compiler.internal.h"

using namespace Reni;

Compiler::Compiler(String const&fileName)
: _internal(new internal(fileName))
{}

p_implementation(Compiler, Ref<Syntax>, syntax)
{
    return _internal->syntaxCache.Value;
};

p_implementation(Compiler, CodeBase, code)
{
    return _internal->codeCache.Value;
};

p_implementation(Compiler, Ref<CodeItem>, main)
{
    return _internal->codeCache.Value.main;
};

p_implementation(Compiler, String, cppCode)
{
    return _internal->cppCode;
};

ExecutionResult Compiler::Execute()
{
    return _internal->Execute();
}

Ref<Syntax> Compiler::GetSyntaxFromText(String const& text)
{
    return internal::GetSyntaxFromText(text);
};


CodeBase::CodeBase(Ref<CodeItem> const& main, Array<Global::Function> const& functions) : main(main), functions(functions)
{
    SetDumpString();
}

p_implementation(CodeBase, String, cppMain){ return MainCodeVisitor::MainVisit(main); };

p_implementation(CodeBase, String, cppFunctions)
{
    return functions
        .Select<String>([&](Global::Function const&function){return function.cppCode; })
        ->Aggregate<String>();
};

p_implementation(CodeBase, String, cppDeclarations)
{
    return functions
        .Select<String>([&](Global::Function const&function){return function.cppDeclarations; })
        ->Aggregate<String>();
};

