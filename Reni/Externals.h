#pragma once

#include "../HWLib/_EditorTemplates.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/CtrlRef.h"
#include "../HWLib/DumpToString.h"
#include "../HWLib/Enumerable.h"
#include "../HWLib/Common.h"

using namespace HWLib;
using namespace std;

namespace Reni
{
    class CodeItem;
    class External; 
    class ReplaceVisitor;

    class Externals final : public DumpableObject
    {
        using baseType = DumpableObject; 
        using thisType = Externals;
        Array<WeakRef<External>> const data;
    public:
        Externals() { SetDumpString(); }
        explicit Externals(External const&item);
        Externals(Externals const&other);
        static Externals const Aggregate(CtrlRef<Enumerable<Externals>> const&other);
    private:
        explicit Externals(Array<WeakRef<External>> const&other);
    public:
        ThisRef;
        DefaultAssignmentOperator;

        p(bool, isEmpty){ return data.Count == 0; }

        Externals const operator+(Externals const&other)const;
        bool const operator==(Externals const&other)const;
        bool const operator==(External const&other)const;

        Optional<thisType> const Replace(ReplaceVisitor const&arg) const;
    private:
        p_function(String, DumpHeader) override { return ""; };
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override{ return Dump; };
    };
}
