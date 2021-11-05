#pragma once

#include <vector>

namespace HWLib
{
    template <typename T>
    class Stack final
    {
        using thisType = Stack;
        std::vector<T> _internal;
    public:
        HW_DO_PLACEMENT_ASSIGN;

        void Push(T target){ _internal.push_back(target); };
        T const Pop(){ auto result = _internal.back(); _internal.pop_back(); return result; };
        p(T, Top) { return _internal.back(); };
        p(bool, IsEmpty){ return _internal.size() == 0; };
    };

}
