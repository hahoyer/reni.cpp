#pragma once
#include "../HWLib/DumpableObject.h"
#include "../Util/Size.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class Address final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Address;
    public:
        Size const data;
        int const depth;

        Address(Size const& data, int depth);

        Address const operator+(int delta)const;
    private:
        p_function(Array<String>, DumpData) override{ return{nd(data), nd(depth)}; };
    };
};