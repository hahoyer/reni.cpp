#include "Import.h"
#include "NumberType.h"

#include "AddressType.h"
#include "ArgVisitor.h"
#include "ArrayType.h"
#include "CodeItems.h"
#include "DefineableToken.h"
#include "ExpressionSyntax.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "Context.h"
#include "Global.h"
#include "Result.h"
#include "VoidType.h"
#include "TemplateInstances.h"
#include "../HWLib/RefCountContainer.instance.h"


using namespace Reni;
static bool Trace = true;

NumberType::NumberType(WeakRef<ArrayType> const parent) 
: parent(*parent){
    SetDumpString();
}

p_implementation(NumberType, Size, size){ return parent.size; };
p_implementation(NumberType, WeakRef<Global>, global) { return parent.global; };

p_implementation(NumberType, Array<String>, DumpData){
    return{nd(parent)};
};

WeakPtr<NumberType> const NumberType::Convert(Type const& target)
{
    auto addressType = dynamic_cast<AddressType const*>(&target);
    if(addressType)
        return Convert(addressType->value);
    
    return dynamic_cast<NumberType *>(&target.thisRef);
}

p_implementation(NumberType, String, DumpShort){
    return base_p_name(DumpShort) + " size=" + size.DumpShort;
}

SearchResult const NumberType::Search(DefineableToken const& token) const
{
    return token.Search(*this);
};

WeakRef<NumberType> const NumberType::Resize(int newSize)const{
    return parent
        .elementType
        .array(newSize)
        ->numberType
        ->thisRef;
}


