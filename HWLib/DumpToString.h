#pragma once

namespace HWLib
{
    class String;
    class DumpableObject;

    template <typename T>
    String const Dump(T const&target);
    template <>
    String const Dump(String const&target);
    template <typename T>
    String const Dump(T const*target);

    String const Dump(int target);
    String const Dump(char const* target);

    template <typename T>
    String const DumpTypeName(T const& object);

    template <class TResult, class T>
    TResult const* const DynamicConvert(T const& object){
        return dynamic_cast<TResult const*>(&object);
    };

    template <class T>
    class Optional; 

    template <class TResult, class T>
    TResult const* const DynamicConvert(Optional<T> const&){
        return null;
    };

    template <class TResult>
    TResult const* const DynamicConvert(int const&){
        return null;
    };
}
    