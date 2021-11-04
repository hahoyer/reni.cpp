#pragma once

namespace HWLib
{
    class DumpableObject;
    template <typename T> class CtrlRef;
    template <typename T> class WeakRef;
    template <typename T> class Ref;

    template <typename T1, typename T2>std::string Dump(std::pair<T1, T2> const&target);
    template <typename T>std::string Dump(CtrlRef<T> const&target);
    template <typename T>std::string Dump(WeakRef<T> const&target);
    template <typename T>std::string Dump(Ref<T> const&target);
    template <typename T>std::string Dump(T const&target);
    template <typename T>std::string DumpShort(T const&target);
    template <typename T>std::string Dump(T const*target);
    template <typename T>std::string Dump(T *target);
    template <typename T>std::string DumpShort(Ref<T> const&target);
    template <typename T>std::string DumpShort(T const*target);
    template <typename T>std::string DumpShort(T *target);
    template <typename T>std::string Dump(Array<T> const&target);
    template <typename T>Array<std::string> DumpData(Array<T> const& target);
    std::string DumpList(Array<std::string> const&target);

    template <>std::string Dump(char const* target);
    std::string Dump(__int64 target, int radix = 10);
    std::string Dump(int target, int radix = 10);
    std::string Dump(size_t target, int radix = 10);
    std::string Dump(bool target);

    template <typename T>std::string DumpTypeName(T const& object);

    template <class TResult, class T>
    TResult const* DynamicConvert(T const& object)
    {
      if constexpr (std::is_convertible_v<T const*, TResult const*>)
        return dynamic_cast<TResult const*>(&object);
      return nullptr;
    };

    template <class T>class Optional; 

    template <class TResult, class T>
    TResult const* DynamicConvert(T const* object)
    {
        return nullptr;
    };

    template <class TResult, class T>
    TResult const* DynamicConvert(Optional<T> const&)
    {
        return {};
    };

    template <class TResult>
    TResult const* DynamicConvert(int const&)
    {
        return nullptr;
    };

}
    