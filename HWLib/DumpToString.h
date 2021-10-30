#pragma once

namespace HWLib
{
    class String;
    class DumpableObject;
    template <typename T> class CtrlRef;
    template <typename T> class WeakRef;
    template <typename T> class Ref;

    template <typename T1, typename T2>String const Dump(std::pair<T1, T2> const&target);
    template <typename T>String const Dump(CtrlRef<T> const&target);
    template <typename T>String const Dump(WeakRef<T> const&target);
    template <typename T>String const Dump(Ref<T> const&target);
    template <typename T>String const Dump(T const&target);
    template <typename T>String const DumpShort(T const&target);
    template <typename T>String const Dump(T const*target);
    template <typename T>String const Dump(T *target);
    template <typename T>String const DumpShort(Ref<T> const&target);
    template <typename T>String const DumpShort(T const*target);
    template <typename T>String const DumpShort(T *target);
    template <typename T>String const Dump(Array<T> const&target);
    template <typename T>Array<String> const DumpData(Array<T> const&target);
    String const DumpList(Array<String> const&target);

    template <>String const Dump(String const&target);
    template <>String const Dump(char const* target);
    String const Dump(__int64 target, int radix = 10);
    String const Dump(int target, int radix = 10);
    String const Dump(size_t target, int radix = 10);
    String const Dump(bool target);

    template <typename T>String const DumpTypeName(T const& object);

    template <class TResult, class T>
    TResult const* const DynamicConvert(T const& object){
        return dynamic_cast<TResult const*>(&object);
    };

    template <class T>class Optional; 

    template <class TResult, class T>
    TResult const* const DynamicConvert(T const* object){
        return null;
    };

    template <class TResult, class T>
    TResult const* const DynamicConvert(Optional<T> const&){
        return {};
    };

    template <class TResult>
    TResult const* const DynamicConvert(int const&){
        return null;
    };

    template <class TResult>
    TResult const* const DynamicConvert(String const&){
        return null;
    };
}
    