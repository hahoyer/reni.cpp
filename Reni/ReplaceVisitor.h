#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni
{
    class ReplaceVisitor : public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef ReplaceVisitor thisType;
    private:
        override_p_function(Array<String>, DumpData){return {};};
    };

}

