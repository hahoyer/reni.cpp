#include "Import.h"
#include "Type.h"

#include "AddressType.h"
#include "ArrayType.h"
#include "BitType.h"
#include "Code.h"
#include "DumpPrintToken.h"
#include "Feature.h"
#include "FeatureProvider.h"
#include "NumberType.h"
#include "Result.h"
#include "TemplateInstances.h"
#include "TypeType.h"
#include "UserDefinedToken.h"

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/FunctionCache.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/ValueCache.h"


using namespace Reni;
static bool Trace = true;

struct Type::internal
{
    FunctionCache<WeakRef<ArrayType>, int> array;
    ValueCache<WeakRef<NumberType>> number;
    ValueCache<WeakRef<TypeType>> type;
    ValueCache<WeakRef<AddressType>> indirect;

    explicit internal(Type const&parent)
        : array([&](int count)
              {
                  return new ArrayType(parent, count);
              })
          , number([&]
              {
                  return parent.CreateNumberType();
              })
          , indirect([&]
              {
                  return new AddressType(parent.thisRef);
              })
          , type([&]
              {
                  return new TypeType(parent.thisRef);
              })
    {
    };
};


Type::Type() : _internal(new internal(*this))
{
}

pure_p_implementation(Type, Size, size) ;
pure_p_implementation(Type, WeakRef<Global>, global) ;
pure_p_implementation(Type, WeakRef<Type>, asFunctionResult) ;
pure_p_implementation(Type, int, addressLevel);

ResultData const Type::GetResultData(Ref<CodeItem> code)const
{
    return ResultData(size, code, thisRef);
};

ResultData const Type::GetResultData(Category category, function<Ref<CodeItem>()> getCode) const
{
    return ResultData(
        !category.hasSize ? Optional<Size>() : size ,
        !category.hasCode ? Ref<CodeItem, true>() : getCode(),
        !category.hasType ? WeakPtr<Type>() : thisRef
    );
}

ResultData const Type::GetResultData() const
{
    a_if(!HasData, Dump);
    return ResultData(size, CodeItem::Const(BitsConst::Empty()), &this->thisRef);
}

WeakRef<Type> const Type::array(int count)const
{
    return _internal->array(count)->thisRef;
};

p_implementation(Type, WeakRef<NumberType>, numberType)
{
    return &_internal->number.Value->thisRef;
};

p_implementation(Type, WeakRef<TypeType>, typeType)
{
    return &_internal->type.Value->thisRef;
};

p_implementation(Type, WeakRef<AddressType>, indirectType)
{
    return &_internal->indirect.Value->thisRef;
};

p_implementation(Type, WeakRef<Type>, asFunctionResult)
{
    md_;
    b_;
    return_d(thisRef);
};

WeakRef<NumberType> const Type::CreateNumberType() const
{
    auto a = dynamic_cast<ArrayType const*>(this);
    a_if(a && dynamic_cast<BitType const*>(&a->elementType), nd(*this) + " cannot be flagged as number type");
    return new NumberType(a->thisRef);
};

ResultData const Type::ContextAccessResult(Category category, Type const& target, function<Size()> getOffset)const
{
    if(!HasData)
        return GetResultData() & category;

    return indirectType
        ->GetResultData
        (category,
         [&]
         {
             return CodeItem::Reference(target)
                 ->ReferencePlus(getOffset());
         });
};

SearchResult const Type::Search(DefineableToken const&token) const
{
    bool Trace = true;
    md(token);
    b_;
    return_d(SearchResult());
};


SearchResult const AddressType::Search(DefineableToken const& token) const
{
    auto result = value->Search(token);
    if(result.IsValid)
        return result;
    return {};
};


SearchResult const TypeType::Search(DefineableToken const& token) const
{
    SearchResult const result = token.Search(*this);
    if(result.IsValid)
        return result;
    return baseType::Search(token);
}



class InstanceFunctionFeature final : public InfixFunctionFeature
{
    using baseType = InfixFunctionFeature;
    using thisType = InstanceFunctionFeature;
    Type const& target;
public:
    InstanceFunctionFeature(Type const& target) : target(target) {}
private:
    ResultData const Result(Category category, Type const&target, Type const&arg)const override
    {
        bool Trace = true;
        md(category, target, arg);
        b_;
        return{};
    }

    p_function(Array<String>, DumpData) override{ return{nd(target)}; }
};


template<>
SearchResult const TypeType::Search<InstanceToken>() const
{
    return new InstanceFunctionFeature(*value);
}

