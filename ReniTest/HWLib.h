#pragma once

namespace _HWLib{
    test_class(FileTest);
    test_class(StackTrace);
}

namespace _Process{
    test_class(Simple);
    test_class(StartProgram);
    test_class(Double);
    test_class(Double2);
    test_class(Error);
}

namespace _String{
    test_class(WriteHallo);
    test_class(Find);
    test_class(Part);
    test_class(Plus);
    test_class(Split);
    test_class(Stringify);
    test_class(Replace);
    test_class(Replace1);
}

namespace _Ref{
    test_class(WriteHallo);
}

namespace _Array{
    test_class(WriteHallo);
}

namespace _Enumerable{
    test_class(FromInt0);
    test_class(FromInt1);
    test_class(FromInt1_1);
    test_class(FromInt1_2);
    test_class(FromInt0_1_1);
    test_class(FromString);
}

namespace _ValueCache{
    test_class(Simple);
    test_class(Context);
    test_class(Member);
}