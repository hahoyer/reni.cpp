#pragma once
#include "Type.h"

namespace Reni
{
    class AddressType final : public Type
    {
        typedef Type baseType;
        typedef AddressType thisType;
    public:
        Type const& value;

        AddressType(Type const& value) : value(value)
        {
            SetDumpString();
            a_if_(!this->value.hollow);
        }
        ThisRef;
    private:
        p_function(Optional<WeakRef<NumberType>>, asNumberType)override{ return value.As<NumberType>(); }
        p_function(Array<string>, DumpData) override{ return{nd(value)}; };
        p_function(WeakRef<Global>, global) override{ return value.global; }
        p_function(bool, hollow) { return false; };
        p_function(WeakRef<Type>, toTypeTarget) override{ return value.toTypeTarget; };
        p_function(Size, size) override{ return Size::Address; }
        p_function(Address, toAddress) override;

        SearchResult<Feature> DeclarationsForType(DeclarationType const& token) const override;
        Array<Ref<FiberItem>> ConvertFiber(Type const& destination) const override;
    };
};


