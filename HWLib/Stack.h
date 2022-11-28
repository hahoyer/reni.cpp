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
        HW_PR_GET(T, Top) { return _internal.back(); };
        HW_PR_GET(bool, IsEmpty){ return _internal.size() == 0; };
    };

}
