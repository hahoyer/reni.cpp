#pragma once
#include "Type.h"

using namespace HWLib;

namespace Reni
{
    class AccessData;
    class ContainerContext;

    class AccessType final : public Type
    {
        using baseType = Type;
        using thisType = AccessType;

        WeakRef<AccessData> data;
    public:
        AccessType(AccessType const&) = delete;
        explicit AccessType(AccessData const& data);
        ThisRef;

        template<class TTokenClass>
        SearchResult<Feature> const DeclarationsForType()const;
    private:
        p(WeakRef<Type>, value);
        p_function(bool, hllw) { return false; };
        p_function(Array<String>, DumpData) override;
        p_function(Size, size)override { return Size::Address; }
        p_function(WeakRef<Global>, global) override;
        WeakPtr<NumberType> const get_asNumberType() const override;
        SearchResult<Feature> const DeclarationsForType(DeclarationType const& token) const override;

    };

};


