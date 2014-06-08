#pragma once
#include "SearchResult.h"

#include "../HWLib/Ref.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Category.h"
#include "../Util/Size.h"

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
        , public SearchTarget<Feature>
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

        ResultData const GetResultData(Category category, function<Ref<CodeItem>()> getCode)const;
        ResultData const GetResultData(Category category, CodeItem const&code)const;
        ResultData const GetResultData(Category category)const;
        ResultData const ContextAccessResult(Category category, Type const&target, std::function<Size()> getOffset)const;
        ResultData const Constructor(Category category, Type const&arg)const;

        virtual SearchResult<Feature> const Search(SearchTarget const&target)const;
        WeakRef<NumberType> const CreateNumberType()const;
        WeakRef<Type> const IndirectType(int depth)const;
        WeakRef<Type> const Common(Type const&other)const;
        bool const isConvertableTo(Type const&other)const;
        
        SearchResult<Feature> const Search(NumberType const&provider) const override;
        SearchResult<Feature> const Search(TypeType const&) const override;
    private:
        p_function(Array<String>,DumpData) override{ return{}; };

    };


    class EnableCutType final : public Type
    {
        typedef Type baseType;
        typedef EnableCutType thisType;
    public:
        WeakRef<Type> value;
        EnableCutType(WeakRef<Type> value) : value(value){ SetDumpString(); }
        ThisRef;
    private:
        p_function(Array<String>, DumpData) override{ return{nd(*value)}; };
        p_function(Size, size) override{ return value->size; }
        p_function(WeakRef<Global>, global) override{ return value->global; }
    };

}
