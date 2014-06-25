#pragma once
#include "../HWLib/DumpableObject.h"
#include "CodeVisitor.h"

using namespace HWLib;

namespace Reni
{
    class CodeFunction final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeFunction;
    public:
        class CodeVisitor;
    private:
        int const index;
        bool const isSetter;
        Ref<CodeItem> const body;
    public:
        CodeFunction()
            : index(-1)
              , isSetter(false)
              , body(CodeItem::Empty())
        {
            SetDumpString();
        }

        static thisType const Getter(int index, Ref<CodeItem> const& body)
        {
            return thisType(false, index, body);
        }

    private:
        CodeFunction(bool isSetter, int index, Ref<CodeItem> const& body)
            : isSetter(isSetter)
              ,index(index)
              , body(body)
        {
            SetDumpString();
            a_if(index >= 0, nd(index));
        }
    public:
        bool const operator==(thisType const&other)const
        {
            return index == other.index
                && isSetter == other.isSetter;
        };

        p(String,cppCode);
    private:
        p_function(Array<String>, DumpData) override
        {
            return
                {
                    nd(isSetter),
                    nd(index),
                    nd(body)
                };
        }
    };
}

namespace HWLib
{
    template<>
    class Constants<Reni::CodeFunction >
    {
    public:
        static Reni::CodeFunction const NotValid;
    };

}
