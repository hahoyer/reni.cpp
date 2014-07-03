#pragma once
#include "../HWLib/WeakRef.h"
#include "../Util/Size.h"

#include "Type.h"
#include <complex>

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
        SearchResult<Feature> const Search() const;
        SearchResult<Feature> const SearchFor(NumberType const& provider) const override;
        WeakRef<NumberType> const Resize(int newSize)const;
        static WeakPtr<NumberType> const Convert(Type const&target);
    private:
        p_function(Size, size) override;
        p_function(WeakRef<Global>, global) override;
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override;

        SearchResult<Feature> const Search(SearchTarget const& token) const override;

        template<class TTokenClass>
        class OperationFeature;

    };
};


