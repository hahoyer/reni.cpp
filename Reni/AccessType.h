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

        Ref<AccessData> data;
    public:
        AccessType(AccessType const&) = delete;
        explicit AccessType(AccessData const& data);
        ThisRef;
    private:
        p(WeakRef<Type>, value);
        p_function(bool, hllw) { return false; };
        p_function(Array<String>, DumpData) override;
        p_function(Size, size)override { return Size::Address; }
        p_function(WeakRef<Global>, global) override;

        SearchResult<Feature> const DeclarationsForType(DeclarationType const& token) const override;
    };

};


