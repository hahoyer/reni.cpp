#pragma once

namespace Reni
{
    class CodeItem : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeItem;
    public:
        CodeItem();
    };

    class CodeView : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeView;
    public:
        CodeView();
    };

}

