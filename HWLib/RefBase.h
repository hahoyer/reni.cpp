#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "Common.h"

namespace HWLib
{
    template<class T, class TDataContainer>
    class RefBase{
        using thisType = RefBase;
    public:
        typedef TDataContainer dataContainerType;
    protected:
        dataContainerType value;

        RefBase() { }
        RefBase(T &value) : RefBase(&value){}
        RefBase(T *value) : value(value){};
        template<typename TOther, class TOtherData>
        RefBase(RefBase<TOther, TOtherData> &other) : RefBase(other.operator*()){};
        RefBase(dataContainerType const&value) : value(value){}

    public:
        DefaultAssignmentOperator;
        T const& operator*()const { return *value; };
        T const* operator->()const { return &*value; };
        T & operator*(){ return *value; };
        T * operator->(){ return &*value; };
        bool const operator==(thisType const&other)const{return value == other.value;};
    };
}

