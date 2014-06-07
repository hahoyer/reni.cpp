#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class External 
        : public WithId<DumpableObject, External>
    {
        using baseType = DumpableObject;
        using thisType = External;

    public:
        class This;
        class Arg;

        ThisRef;
        bool const operator < (thisType const& other)const;
        virtual bool const IsProvided(ReplaceVisitor const&arg) const = 0;
    private:
        p_function(Array<String>, DumpData) override{ return{}; };
        p_function(String, DumpHeader) override;
    };

    class External::This final : public thisType
    {
        using baseType = External;
        using thisType = This;
    public:
        static thisType const Instance;
    private:
        bool const IsProvided(ReplaceVisitor const& arg) const override;
    };

    class External::Arg final : public thisType
    {
        using baseType = External;
        using thisType = Arg;
    public:
        static thisType const Instance;
    private:
        bool const IsProvided(ReplaceVisitor const& arg) const override;
    };
}
