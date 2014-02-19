#pragma once

#include "DefaultAssignmentOperator.h"
#include <functional>
#include <string.h>
#include <unordered_map>
#include "Properties.h"

using std::tr1::function;
using std::unordered_map;

namespace HWLib
{
    template <typename TKey, typename TValue>
    class FunctionCacheBase{
        using thisType = FunctionCacheBase;
    public:
        struct traits{ static TValue PendingFindValue(); };
    private:
        function<TValue(TKey)> const _createValue;
        mutable unordered_map<TKey, TValue> _data;
        TValue const PendingFindValue;
    protected:
        FunctionCacheBase(FunctionCacheBase<TKey, TValue> const& x) = delete;
        FunctionCacheBase(function<TValue(TKey)> createValue)
            : _createValue(createValue)
            , PendingFindValue(traits::PendingFindValue()){};
        FunctionCacheBase(TValue pendingFindValue, function<TValue(TKey)> createValue)
            : PendingFindValue(pendingFindValue)
            , _createValue(createValue){}
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
            return _data.find(key)->second;
        };

        bool const IsValid(TKey const key) const{ return _data.find() != _data.end(); }

        void IsValid(TKey const key, bool value)const{
            if (value)
                Ensure(key);
            else
                Reset(key);
        }

    private:
        void Ensure(TKey const key)const{
            auto element = _data.find(key);
            if (element == _data.end()){
                _data.insert(std::pair<TKey, TValue>(key, PendingFindValue));
                auto result = _createValue(key);
                a_if_(::memcmp(&result, &PendingFindValue, sizeof(PendingFindValue)));
                _data.at(key) = result;
            }
        }
            
        void Reset(TKey const key)const{
            auto element = _data.find(key);
            if (element != _data.end())
                _data.remove(key);
        }
    };


    template<typename TValue>
    struct FunctionCacheTraits{
        static TValue PendingFindValue(); 
    };

    template<typename TKey, typename TValue>
    TValue FunctionCacheBase<TKey, TValue>::traits::PendingFindValue(){
        return FunctionCacheTraits<TValue>::PendingFindValue();
    }


};