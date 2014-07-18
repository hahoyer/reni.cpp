#pragma once
#include "SearchResult.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Category.h"

using namespace HWLib;
using namespace Util;
using namespace Reni;

template <class TFeature>
SearchResult<TFeature> const SearchResult<TFeature>::operator+(Type const& fromType)const
{
    return SearchResult<TFeature>(feature, path + WeakRef<Type>(fromType.thisRef));
}

template <class TFeature>
ResultData const SearchResult<TFeature>::FunctionResult(
    Context const&context,
    Category category,
    Optional<Ref<Syntax>> const& right
    )const
{
    return feature.FunctionResult(context, category, right);
};

template <class TFeature>
ResultData const SearchResult<TFeature>::FunctionResult(
    Context const&context,
    Category category,
    Optional<Ref<Syntax>> const& left,
    Optional<Ref<Syntax>> const& right
    )const
{
    return feature.FunctionResult(context, category, left, right);
};

template <class TFeature>
ResultData const SearchResult<TFeature>::ConversionResult(Category category, Type const& target, Type const& destination) const
{
    return feature.ConversionResult(category, target, destination);
};

