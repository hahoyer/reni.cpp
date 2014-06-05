#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class External final
        : public WithId<DumpableObject, External>
    {
        using baseType = DumpableObject;
        using thisType = External;

    public:
        static External const Arg;
        static External const This;

        ThisRef;
        bool const operator < (External const& other)const;
    private:
        p_function(Array<String>, DumpData) override{ return{}; };
        p_function(String, DumpHeader) override;
    };

}
