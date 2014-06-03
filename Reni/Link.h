#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni
{
    class Link final 
        : public DumpableObject
        , public RefCountProvider
    {
        using baseType = DumpableObject; 
        using thisType = Link;

    };
}