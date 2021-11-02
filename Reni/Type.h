#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "../HWLib/Ref.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Category.h"
#include "../Util/Size.h"
#include "Result.h"
#include "SearchTarget.h"
#include "Feature.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class Address;
    class ArrayType;
    class CodeItem;
    class ContextReference;
    class DefineableToken;
    class DefinitionPoint;
    class DumpPrintToken;
    class EnableCutType;
    class Externals;
    class FiberItem;
    class FunctionToken;
    class Global;
    class InstanceToken;
    class NumberType;
    class PlusToken;
    class ResultData;
    class StarToken;
    class TypeType;

    class Type
        : public WithId<DumpableObject, Type>
        , public DeclarationType
    {
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
        struct internal;
        CtrlRef<internal> _internal;
    protected:
        Type();
    public:
        Type(Type const&) = delete;
        ThisRef;

        bool operator==(Type const& other)const{return this == &other;}

        p_virtual(bool, hllw) = 0;
        p_virtual(Size, size) = 0;
        p_virtual(WeakRef<Global>, global) = 0;
        p_virtual_definition(WeakRef<Type>,toTypeTarget);
        p_virtual_function(WeakRef<Type>,toTypeTarget);
        virtual WeakRef<Type> const get_toTypeTarget()const{return thisRef;};
        p_virtual(Address, toAddress);
        p_virtual(bool, isCopyable) { return true; };

        WeakRef<Type> const array(size_t count)const;
        p(WeakRef<NumberType>, numberType);
        p_definition(WeakRef<TypeType>,typeType); WeakRef<TypeType> const get_typeType()const;
        p(WeakRef<Type>, indirectType);
        p(WeakRef<EnableCutType>, enableCutType);
        p_virtual(WeakRef<Type>, asFunctionResult);
    protected:
        p_virtual(Optional<WeakRef<NumberType>>, asNumberType){ return{}; };
    public:
        template <class TDestination>
        Optional<WeakRef<TDestination>> const As()const;

        template <>
        Optional<WeakRef<NumberType>> const As()const{return asNumberType;}

        ResultData const GetResultData(Category category, function<Ref<CodeItem>()> getCode, function<Externals()> getExts)const;
        ResultData const GetResultDataSmartExts(Category category, function<Ref<CodeItem>()> getCode)const;
        ResultData const GetResultDataEmpty(Category category)const;

        virtual SearchResult<Feature> const DeclarationsForType(DeclarationType const& target)const;
        WeakRef<NumberType> const CreateNumberType()const;
        WeakRef<Type> const IndirectType(int depth)const;
        WeakRef<Type> const Common(Type const& other)const;
        bool const isConvertableTo(Type const& other)const;
        Ref<ResultCache> const ConvertTo(Type const& destination) const;
        virtual Ref<ResultCache> const DirectConvert() const;
        SearchResult<Feature> const Declarations(NumberType const& provider) const override;
        SearchResult<Feature> const Declarations(TypeType const&) const override;
        SearchResult<Feature> const Declarations(EnableCutType const&) const override;
        SearchResult<Feature> const Declarations(AccessType const&) const override;
        virtual Array<Ref<FiberItem>> const ConvertFiber(Type const& destination) const;
    private:
        p_function(Array<String>,DumpData) override
        {
            return{};
        };

    };


    class EnableCutType final : public Type
    {
        typedef Type baseType;
        typedef EnableCutType thisType;
    public:
        Type const& value;

        EnableCutType(Type const& value) : value(value)
        {
            SetDumpString();
            a_if_(!this->value.hllw);
        }

        ThisRef;

    private:
        p_function(Array<String>, DumpData) override{return{nd(value)};};
        p_function(bool, hllw) { return false; };
        p_function(Size, size) override{ return value.size; }
        p_function(WeakRef<Global>, global) override{return value.global;}
        p_function(Optional<WeakRef<NumberType>>, asNumberType) override{return value.As<NumberType>();}
        p_function(Address, toAddress) override;
        SearchResult<Feature> const DeclarationsForType(DeclarationType const& target) const override;
    };
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
