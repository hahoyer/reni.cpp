#pragma once
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{
    class ArrayType;


    class Feature
        : public DumpableObject
        , public RefCountProvider{
        typedef DumpableObject baseType;
        typedef Feature thisType;
        override_p_function(Array<String>, DumpData) {
            return{};
        }
    };
}
