#pragma once
#include "Type.h"
#include "../Util/Size.h"

using namespace HWLib;
using namespace Util;


namespace Reni
{
    class TypeType final : public Type
    {
        typedef Type baseType;
        typedef TypeType thisType;
        WeakRef<Type> value;
    public:
        TypeType(WeakRef<Type> value) : value(value){}
        ThisRef;
    private:
        p_function(Array<String>, DumpData) override{return{ nd(value) };};
        p_function(Size, size) override{return 0;}
        p_function(WeakRef<Global>, global) override{return value->global;}
        operator HWLib::Ref<Reni::FeatureProvider<InstanceToken>, true>() const override;;
    };
}