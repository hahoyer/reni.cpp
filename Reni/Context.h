#pragma once

namespace Reni
{
    class Context : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Context;
    public:
        Context();
    };

    class RootContext : public Context
    {
        using baseType = Context;
        using thisType = RootContext;
    public:
        RootContext();
    private:
        override_p_function(Array<String>, DumpData);
    };

}

