#pragma once

namespace Reni
{
    class Syntax : public DumpableObject
    {
    public:
        Syntax(){ SetDumpString(); }


    private:
        virtual p_function(Array<String>, DumpData)override;
    };

}

template<>              
static inline bool HWLib::Features<Reni::Syntax const>::EnableDumpFromRef(){ return true; };

template<>
inline String const Features<Reni::Syntax const>::DumpToStringShort(Reni::Syntax const&value){ return value.DumpShort; };

