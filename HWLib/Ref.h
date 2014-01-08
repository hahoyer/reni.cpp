#pragma once

#include "DefaultAssignmentOperator.h"
#include "Properties.h"

namespace HWLib
{
    template<typename T> class Ref
    {
        T const*const _data;
    public:
        Ref(T const& data) : _data(data){ OnCopy(); };
        Ref(Ref<T> const& data) : _data(data._data){ OnCopy(); };

        ~Ref(){ _data->RemoveRef(); };

        DefaultAssignmentOperator(Ref<T>);

        p(T const*, RawData){ return _data; }

        T const& operator*()const { assert(_data); return *_data; };
        T const* operator->()const { assert(_data); return _data; };
        operator T const&()const { assert(_data); return *_data; };
        template<class T2>
        bool operator==(const Ref<T2>&x)const { return _data == x._data; };
        template<class T2>
        bool operator==(T2 const*x)const { return _data == x; };
    private:
        void OnCopy(){ _data->AddRef(); }
    };

    template<typename T> class RefCounted
    {
        mutable T count;
    public:
        RefCounted();
        RefCounted(RefCounted const&x);
        virtual ~RefCounted();

        void* operator new(size_t size);
        void* operator new (size_t s, void*p);
        void operator delete(void*);

        void AddRef() const; // Is const, since referencing is not realy a modifying operation
        void RemoveRef() const;  // Is const, since removing a rerence is not realy a modifying operation

        p(bool, IsAutomatic) { return count == -1; };

    private:
        void Initialize();
    };

    template class RefCounted<__int32>;
    template class RefCounted<__int64>;

}
