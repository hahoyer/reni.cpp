#pragma once

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

namespace Reni{
    class AddressType;
    class ArrayType;
    class CodeItem;
    class ContextReference;
    class DefineableToken;
    class DefinitionPoint;
    class DumpPrintToken;
    class EnableCutType;
    class Externals;
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
        bool operator==(Type const&other)const{ return this == &other; }

        p(bool, HasData){ return size != 0; };
        virtual_p(Size, size) = 0;
        virtual_p(WeakRef<Global>, global) = 0;
        virtual_p(int, addressLevel) { return 0; };
        virtual_p(bool, isTypeTarget) { return true; };
        virtual_p(WeakRef<Type>, dereferencedType) { return thisRef; };

        WeakRef<Type> const array(int count)const;
        p(WeakRef<NumberType>, numberType);
        p(WeakRef<TypeType>, typeType);
        p(WeakRef<Type>, indirectType);
        p(WeakRef<EnableCutType>, enableCutType);
        virtual_p(WeakRef<Type>, asFunctionResult);
    protected:
        virtual_p(WeakPtr<NumberType>, asNumberType){return{};};
    public:
        template <class TDestination>
        WeakPtr<TDestination> const As()const;
        template <>
        WeakPtr<NumberType> const As()const{ return asNumberType; }

        ResultData const GetResultData(Category category, function<Ref<CodeItem>()> getCode, function<Externals()> getExts)const;
        ResultData const GetResultDataSmartExts(Category category, function<Ref<CodeItem>()> getCode)const;
        ResultData const GetResultDataEmpty(Category category)const;
        ResultData const ContextAccessResult(Category category, Type const&target, std::function<Size()> getOffset)const;

        virtual SearchResult<Feature> const DeclarationsForType(DeclarationType const&target)const;
        WeakRef<NumberType> const CreateNumberType()const;
        WeakRef<Type> const IndirectType(int depth)const;
        WeakRef<Type> const Common(Type const&other)const;
        bool const isConvertableTo(Type const&other)const;
        ResultData const ConvertTo(Category category, Type const& other) const;
        SearchResult<Feature> const Declarations(NumberType const&provider) const override;
        SearchResult<Feature> const Declarations(TypeType const&) const override;
        SearchResult<Feature> const Declarations(EnableCutType const&) const override;
    private:
        p_function(Array<String>,DumpData) override{ return{}; };
    };


    class EnableCutType final : public Type
    {
        typedef Type baseType;
        typedef EnableCutType thisType;
    public:
        Type const& value;
        EnableCutType(Type const&value) : value(value){ SetDumpString(); }
        ThisRef;
    private:
        p_function(Array<String>, DumpData) override{ return{nd(value)}; };
        p_function(Size, size) override{ return value.size; }
        p_function(WeakRef<Global>, global) override{ return value.global; }
        p_function(WeakPtr<NumberType>, asNumberType)override{ return value.As<NumberType>(); }
        SearchResult<Feature> const DeclarationsForType(DeclarationType const& target) const override;
    };

}
