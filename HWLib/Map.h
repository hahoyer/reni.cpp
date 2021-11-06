#pragma once
#include "Ref.h"
#include <unordered_map>
#include "_.h"
#include "Optional.h"

namespace HWLib
{
    template<class TKey, class TValue>
    class Map
    {
        using thisType = Map;
        typedef std::unordered_map<TKey, TValue> dataType;
    public:
        typedef TKey keyType;
    private:
        dataType data;
    public:
        bool const ContainsKey(TKey const& key)const
        {
            return data.find(key) != data.end();
        }

        p(Array<TKey>, keys)
        {
            return _(data)
                .ToEnumerable<dataType::value_type>()
                .Select<keyType>([&](dataType::value_type const& pair)
                    {
                        return pair.first;
                    })
                ->ToArray;
        }

        TValue const operator[](TKey const& key)const
        {
            auto result = data.find(key);
            HW_ASSERT_(result != data.end());
            return result->second;
        };

        void Assign(TKey const& key, TValue value)
        {
            data.insert(dataType::value_type(key, value));
        };

        Optional<TValue> const Find(TKey const& key)const
        {
            auto result = data.find(key);
            if (result == data.end())
                return{};
            return result->second;
        };
    };

}