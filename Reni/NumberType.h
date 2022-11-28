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
        HW_PR_THISREF;

        template<class TTokenClass>
        SearchResult<Feature> const DeclarationsForType() const;
        SearchResult<Feature> const Declarations(NumberType const& provider) const override;
        SearchResult<Feature> const Declarations(EnableCutType const& provider) const override;
        WeakRef<NumberType> const Resize(size_t newSize)const;
    private:
        HW_PR_DECL_GETTER(bool, hollow) { return false; };
        HW_PR_DECL_GETTER(Size, size) override;
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override;
        HW_PR_DECL_GETTER(Array<string>, DumpData) override;
        HW_PR_DECL_GETTER(string, DumpShort) override;
        HW_PR_DECL_GETTER(Optional<WeakRef<NumberType>>, asNumberType)override{ return WeakRef<NumberType>(thisRef); }

        SearchResult<Feature> DeclarationsForType(DeclarationType const& token) const override;

        template<class TTokenClass>
        class OperationFeature;
        template<class TTokenClass>
        WeakRef<Type> const ResultType(size_t other)const;

    };
};


