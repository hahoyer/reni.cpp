#include "Import.h"
#include "SyntaxContainer.h"

using namespace Reni;
static bool Trace = true;


SyntaxContainer::SyntaxContainer(SourcePart const&part) : baseType(part){
}

p_implementation(SyntaxContainer, Array<String>, DumpData){
    return{};
};

void SyntaxContainer::Add(Ref<Syntax> const& definitionTarget, Ref<Syntax> const& value){
    md(definitionTarget, value);
    b_;
}