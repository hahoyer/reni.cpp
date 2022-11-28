#pragma once

#include "DefaultOperators.h"
#include <functional>
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include "Properties.h"

namespace HWLib
{
  template <typename TValue, typename... TKey>
  class FunctionCacheBase
  {
    using thisType = FunctionCacheBase;

  private:
    function<TValue(TKey...)> const createValue;
    typedef std::tuple<TKey...> TKeys;
    mutable std::unordered_map<TKeys, TValue> data;
    mutable std::unordered_set<TKeys> busyKeys;

  protected:
    FunctionCacheBase(FunctionCacheBase<TValue, TKey...> const& x) = delete;

    FunctionCacheBase(function<TValue(TKey...)> createValue)
      : createValue(createValue)
    { };

  public:
    class KeyNotFoundException
    {
      TKeys const _key;

    public:
      KeyNotFoundException(TKeys const& key)
        : _key(key)
      {
        b(Message);
      };
      HW_PR_GET(std::string, Message) { return "Key not found: " + ::dump(_key); }
    };

    static TValue ThrowKeyNotFoundException(TKeys key) { throw KeyNotFoundException(key); };

    TValue const operator()(TKey... key) const
    {
      Ensure(key...);
      return data.find(TKeys(key...))->second;
    };

    bool const IsValid(TKey... key) const { return data.find(TKeys(key...)) != data.end(); }

    void IsValid(TKey... key, bool value) const
    {
      if(value)
        Ensure(key...);
      else
        Reset(key...);
    }

  private:
    void Ensure(TKey... key) const
    {
      auto element = data.find(TKeys(key...));
      if(element == data.end())
      {
        HW_ASSERT_AND_THROW(busyKeys.find(TKeys(key...)) == busyKeys.end(), "illegal recursion");
        busyKeys.insert(TKeys(key...));
        auto result = createValue(key...);
        busyKeys.erase(TKeys(key...));
        data.insert(std::pair<TKeys, TValue>(TKeys(key...), result));
      }
    }

    void Reset(TKey... key) const
    {
      auto element = data.find(TKeys(key...));
      if(element != data.end())
        data.erase(TKeys(key...));
    }
  };
};

namespace std
{
  template <class TKey>
  struct hash<tuple<TKey>>
  {
    size_t operator()(tuple<TKey> const& key) const
    {
      return hash<TKey>()(get<0>(key));
    }
  };

  template <class TKey0, class TKey1, class... TKeys>
  struct hash<tuple<TKey0, TKey1, TKeys...>>
  {
    size_t operator()(tuple<TKey0, TKey1, TKeys...> const& key) const
    {
      return hash<TKey0>()(get<0>(key)) + 2 * hash<TKey1>()(get<1>(key));
    }
  };
}
