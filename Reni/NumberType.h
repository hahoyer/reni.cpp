#pragma once
#include "../HWLib/WeakRef.h"
#include "../Util/Size.h"

#include "Type.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class EnableCutToken;
    class Syntax;

    class NumberType final : public Type
    {
        typedef Type baseType;
        typedef NumberType thisType;
        ArrayType const& parent;
    public:
        explicit NumberType(WeakRef<ArrayType> const parent);
        NumberType(NumberType const&) = delete;
        ThisRef;

        template<class TTokenClass>
        SearchResult<Feature> const DeclarationsForType() const;
        SearchResult<Feature> const Declarations(NumberType const& provider) const override;
        SearchResult<Feature> const Declarations(EnableCutType const& provider) const override;
        WeakRef<NumberType> const Resize(size_t newSize)const;
    private:
        p_function(bool, hllw) { return false; };
        p_function(Size, size) override;
        p_function(WeakRef<Global>, global) override;
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override;
        p_function(Optional<WeakRef<NumberType>>, asNumberType)override{ return thisRef; }

        SearchResult<Feature> const DeclarationsForType(DeclarationType const& token) const override;

        template<class TTokenClass>
        class OperationFeature;
        template<class TTokenClass>
        WeakRef<Type> const ResultType(size_t other)const;

    };
};


