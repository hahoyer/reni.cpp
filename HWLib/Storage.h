#pragma once

namespace HWLib
{
    class CStorage
    {
    public:
        void* Allocate(size_t byteCount){ return new __int8[byteCount]; }
        void Free(void* data){ delete[] reinterpret_cast<__int8*>(data); }
    };

    static CStorage Heap;
}

inline void *__cdecl operator new(size_t byteCount, ::HWLib::CStorage& p){ return p.Allocate(byteCount); };
inline void __cdecl operator delete(void * data, ::HWLib::CStorage& p){ p.Free(data); };

#define HeapAllocate *new(::HWLib::Heap)
#define HeapFree delete&
