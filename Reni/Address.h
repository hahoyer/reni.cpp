#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class Type;

    class Address final : public DumpableObject
    {
        using baseType = HWLib::DumpableObject;
        using thisType = Address;
    public:
        Type const& data;
        int const depth;

        Address(Address const& other);
        Address(Type const& data, int depth);

        Address const operator+(int delta)const;
    private:
        p_function(Array<String>, DumpData) override{ return{nd(data), nd(depth)}; };
    };
};