#pragma once

#include "DefaultAssignmentOperator.h"
#include <functional>
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include "Properties.h"

namespace HWLib
{
    template <typename TValue, typename TKey>
    class FunctionCacheBase{
        using thisType = FunctionCacheBase;
    private:
        function<TValue(TKey)> const createValue;
        mutable std::unordered_map<TKey, TValue> data;
        mutable std::unordered_set<TKey> busyKeys;
    protected:
        FunctionCacheBase(FunctionCacheBase<TValue, TKey> const& x) = delete;
        FunctionCacheBase(function<TValue(TKey)> createValue)
            : createValue(createValue){};
    public:
        class KeyNotFoundException{
            TKey const _key;
        public:
            KeyNotFoundException(TKey const &key)
                : _key(key){
                b(Message);
            };
            p(String, Message){ return "Key not found: " + ::dump(_key); }
        };

        static TValue ThrowKeyNotFoundException(TKey key){ throw KeyNotFoundException(key); };

        TValue const operator[](TKey const key)const{
            Ensure(key);
            return data.find(key)->second;
        };

        bool const IsValid(TKey const key) const{ return data.find() != data.end(); }

        void IsValid(TKey const key, bool value)const{
            if (value)
                Ensure(key);
            else
                Reset(key);
        }

    private:
        void Ensure(TKey const key)const{
            auto element = data.find(key);
            if (element == data.end()){
                a_throw(busyKeys.find(key) == busyKeys.end(), "illegal recursion");
                busyKeys.insert(key);
                auto result = createValue(key);
                busyKeys.erase(key);
                data.insert(std::pair<TKey, TValue>(key, result));
            }
        }
            
        void Reset(TKey const key)const{
            auto element = data.find(key);
            if (element != data.end())
                data.remove(key);
        }
    };
};