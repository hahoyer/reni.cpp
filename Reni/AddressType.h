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
            a_if_(this->value.size.value);
        }
        ThisRef;
    private:
        p_function(WeakPtr<NumberType>, asNumberType)override{ return value.As<NumberType>(); }
        p_function(Array<String>, DumpData) override{ return{nd(value)}; };
        p_function(WeakRef<Global>, global) override{ return value.global; }
        p_function(bool, isData) { return false; };
        p_function(bool, isTypeTarget) override{ return false; };
        p_function(Size, size) override{ return Size::Address; }
        p_function(Address, toAddress);

        SearchResult<Feature> const DeclarationsForType(DeclarationType const& token) const override;
    };
};


