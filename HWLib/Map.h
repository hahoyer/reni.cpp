#pragma once
#include "Ref.h"
#include "CtrlPtr.h"
#include <unordered_map>
#include "_.h"
#include "Optional.h"

namespace HWLib{
    template<class TKey, class TValue, class TOptionalValue = TValue>
    class MapBase{
        typedef MapBase thisType;
        typedef std::unordered_map<TKey, TValue> dataType;
    public: 
        typedef TKey keyType;
    private:
        dataType data;
    public:
        bool const ContainsKey(TKey const&key)const{
            return data.find(key) != data.end();
        }
        p(Array<TKey>, keys){
            return _(data)
                .ToEnumerable<dataType::value_type>()
                .Select<keyType>([&](dataType::value_type const&pair){return pair.first; })
                ->ToArray;
        }
    protected:
        TValue const operator[](TKey const&key)const{
            auto result = data.find(key);
            a_if_(result != data.end());
            return result->second;
        };

        void Assign(TKey const&key, TValue value){
            data.insert(dataType::value_type(key, value));
        };

        TOptionalValue const Find(TKey const&key)const{
            auto result = data.find(key);
            if(result == data.end())
                return TOptionalValue();
            return result->second;
        };
    };


    template<class TKey, class TValue>
    class Map final: public MapBase<TKey, TValue>{
        typedef Map thisType;
        typedef MapBase<TKey, TValue, TValue> baseType;
    public:
        TValue const operator[](TKey const&key)const{return baseType::operator[](key);};
        void Assign(TKey const&key, TValue value){baseType::Assign(key, value);};
    };


    template<class TKey, class TValue>
    class Map<TKey, Ref<TValue>> final : public MapBase<TKey, CtrlRef<Ref<TValue>>, CtrlPtr<Ref<TValue>>>{
        typedef Map thisType;
        typedef MapBase<TKey, CtrlRef<Ref<TValue>>, CtrlPtr<Ref<TValue>>> baseType;
    public:
        Ref<TValue> const operator[](TKey const&key)const{ return *baseType::operator[](key); };
        void Assign(TKey const&key, Ref<TValue> value){ baseType::Assign(key, new Ref<TValue>(value)); };

        Optional<Ref<TValue>> const Find(TKey const&key)const{
            auto result = baseType::Find(key);
            if(result.IsEmpty)
                return{};
            return *result;
        };
    };

}