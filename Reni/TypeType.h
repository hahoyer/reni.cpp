#pragma once
#include "Feature.h"
#include "Type.h"
#include "../Util/Size.h"

using namespace HWLib;
using namespace Util;


namespace Reni
{
    class InstanceToken;
    class SearchResult;

    class TypeType final : public Type
    {
        typedef Type baseType;
        typedef TypeType thisType;
    public:
        WeakRef<Type> value;
        TypeType(Type const& value);
        ThisRef;

        template<class TTokenClass>
        SearchResult const Search() const;
        static WeakPtr<TypeType> const Convert(Type const&target);
    private:
        p_function(Array<String>, DumpData) override{return{ nd(*value) };};
        p_function(Size, size) override{return 0;}
        p_function(WeakRef<Global>, global) override{return value->global;}
        SearchResult const Search(DefineableToken const& token) const override;
    };
};

using namespace Reni;

template<>
SearchResult const TypeType::Search<InstanceToken>() const;

