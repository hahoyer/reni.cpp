#pragma once

#include "DefaultAssignmentOperator.h"
#include <functional>
#include <map>
#include "Properties.h"

using std::tr1::function;
using std::map;

namespace HWLib
{
    template <typename TKey, typename TValue>
    class FunctionCache final
    {
        using thisType = FunctionCache;
        function<TValue(TKey)> const _createValue;
        mutable map<TKey, TValue> _data;

        TValue const PendingFindValue;

    public:
        FunctionCache(FunctionCache<TKey, TValue> const& x) = delete;

        FunctionCache(function<TValue(TKey)> createValue)
            : _createValue(createValue)
            , PendingFindValue(empty)
        {
        };

        FunctionCache(TValue pendingFindValue, function<TValue(TKey)> createValue)
            : PendingFindValue(pendingFindValue)
            , _createValue(createValue)
        {
        }

        DefaultAssignmentOperator;

        class KeyNotFoundException
        {
            TKey const _key;
        public:
            KeyNotFoundException(TKey const &key)
                : _key(key)
            {
                b(Message);
            };
            p(String, Message){ return "Key not found: " + ::dump(_key); }
        };

        static TValue ThrowKeyNotFoundException(TKey key){ throw KeyNotFoundException(key); };

        TValue const operator[](TKey const key)const
        {
            Ensure(key);
            return _data.find(key)->second;
        };

        bool const IsValid(TKey const key) const{ return _data.find() != _data.end(); }

        void IsValid(TKey const key, bool value)
        {
            if (value)
                Ensure(key);
            else
                Reset(key);
        }

    private:
        void Ensure(TKey const key)
        {
            auto element = _data.find(key);
            if (element == _data.end())
            {
                _data.insert(std::pair<TKey, TValue>(key, PendingFindValue));
                auto result = _createValue(key);
                a_is_(result, !=, PendingFindValue);
                _data.at(key) = result;
            }
        }
            
        void Reset(TKey const key)
        {
            auto element = _data.find(key);
            if (element != _data.end())
                _data.remove(key);
        }
    };
};