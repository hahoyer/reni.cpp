#pragma once

namespace TestHWLib{
    HW_TM_CLASS(FileTest);
    HW_TM_CLASS(StackTrace);
}

namespace TestProcess{
    HW_TM_CLASS(Simple);
    HW_TM_CLASS(StartProgram);
    HW_TM_CLASS(Double);
    HW_TM_CLASS(Double2);
    HW_TM_CLASS(Error);
}

namespace TestString{
    HW_TM_CLASS(WriteHallo);
    HW_TM_CLASS(Find);
    HW_TM_CLASS(Part);
    HW_TM_CLASS(Plus);
    HW_TM_CLASS(Split);
    HW_TM_CLASS(Stringify);
    HW_TM_CLASS(Replace);
    HW_TM_CLASS(Replace1);
}

namespace TestRef{
    HW_TM_CLASS(WriteHallo);
}

namespace TestArray{
    HW_TM_CLASS(WriteHallo);
}

namespace TestEnumerable{
    HW_TM_CLASS(FromInt0);
    HW_TM_CLASS(FromInt1);
    HW_TM_CLASS(FromInt1_1);
    HW_TM_CLASS(FromInt1_2);
    HW_TM_CLASS(FromInt0_1_1);
    HW_TM_CLASS(FromString);
}

namespace TestValueCache{
    HW_TM_CLASS(Simple);
    HW_TM_CLASS(Context);
    HW_TM_CLASS(Member);
}

namespace TestFunctionCache{
    HW_TM_CLASS(Simple);
    HW_TM_CLASS(Context);
    HW_TM_CLASS(Multiple);
}