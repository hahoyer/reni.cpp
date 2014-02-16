#pragma once

namespace HWLib
{
    class String;
    class DumpableObject;

    template <typename T>
    String const Dump(T const&target);
    template <>
    String const Dump(String const&target);

    String const Dump(int target);
    String const Dump(char const* target);

    template <typename T>
    String const DumpTypeName(T const& object);

    template <class TResult, class T>
    TResult const* const DynamicConvert(T const& x){
        return dynamic_cast<TResult const*>(&x);
    };

    template <class TResult>
    TResult const* const DynamicConvert(int const&){
        return 0;
    };
}
    