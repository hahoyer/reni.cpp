#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
#include "RefBase.h"

namespace HWLib
{
    template<typename T>class Ref;

    template<typename T>
    class OptRef final : public RefBase<T, boost::shared_ptr<T>>{
        typedef RefBase<T, boost::shared_ptr<T>> baseType;
        typedef OptRef thisType;
        friend class Ref<T>;
        friend class OptRef<T>;
    public:
        OptRef() : baseType() { }
        OptRef(T *value) :baseType(value){ }
        OptRef(OptRef<T> const&other) : baseType(other){ };
        OptRef(Ref<T> const&other) : baseType(other.value){ };
        template<typename TOther>
        OptRef(OptRef<TOther> const&other) : baseType(other){ };

        DefaultAssignmentOperator;

        p(bool, IsValid){ return baseType::IsValid; }
    };
}

