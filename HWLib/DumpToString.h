#pragma once

namespace HWLib
{
    class String;
    class DumpableObject;
    template <typename T> class Ref;
    template <typename T> class OptRef;
    template <typename T> class WeakRef;
    template <typename T> class OptWeakRef;

    template <typename T>String const Dump(OptRef<T> const&target);
    template <typename T>String const DumpShort(OptRef<T> const&target);
    template <typename T>String const Dump(Ref<T> const&target);
    template <typename T>String const Dump(OptWeakRef<T> const&target);
    template <typename T>String const Dump(WeakRef<T> const&target);
    template <typename T>String const Dump(T const&target);
    template <typename T>String const DumpShort(T const&target);
    template <typename T>String const Dump(T const*target);
    template <typename T>String const Dump(T *target);
    template <typename T>String const DumpShort(T const*target);
    template <typename T>String const DumpShort(T *target);

    template <>String const Dump(String const&target);
    template <>String const Dump(char const* target);
    String const Dump(int target);
    String const Dump(size_t target);
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
        return null;
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
    