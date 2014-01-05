#pragma once
// Placement operator
#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline void *__cdecl operator new(size_t, void *p){return p;};
inline void __cdecl operator delete(void *, void *){};
#endif

