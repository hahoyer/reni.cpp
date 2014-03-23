#pragma once
#include "../HWLib/String.h"
#include "../HWLib/RuntimeChain.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/DumpToString.h"

#define test_class_name(name) name 
#define test_instance_name(name) _##name 
#define test_class(name) \
struct test_class_name(name) final: public HWTest::TestFixture::base{\
    String const location()const override {return FILE_LINE_FUNCTION;}\
    void Run() const override;\
}
#define test_instance(name) HWLib::RuntimeChain<HWTest::TestFixture> test_instance_name(name)(new test_class_name(name) ());
#define test_implementation(name) void test_class_name(name)::Run() const
#define test(name,...) test_class(name); test_instance(name); test_implementation(name)
#define test_item(name) (*test_instance_name(FileTest).data)
using namespace HWLib;


namespace HWTest{
    struct TestFailedException : public DumpableObject{
        virtual~TestFailedException(){}
    };

    struct TestFixture{
        struct base{
            Array<CtrlRef<base>> dependencies;
            bool isLowPriority;
            base() : isLowPriority(false){};
            virtual void Run()const = 0;
            virtual String const location()const = 0;
        };

        typedef base* dataType;
        static base* currentTest;

        static void RunAll(){
            auto all = RuntimeChain<TestFixture>::All()
                ->Sort(HasHigherPriority)
                ->ToArray;
            for(int i = 0; i < all.Count; i++){
                currentTest = all[i];
                auto name = typeid(*currentTest).name();
                _console_ WriteLine(currentTest->location());
                _console_ IndentLevel++;
                try{
                    currentTest->Run();
                }
                catch(TestFailedException const&exception){
                    _console_ IndentLevel--;
                    _console_ WriteLine(String("test(") + name + ") exception: ");
                    _console_ IndentLevel++;
                    _console_ WriteLine(HWLib::Dump(exception));
                }
                catch(...){
                    _console_ IndentLevel--;
                    _console_ WriteLine(String("test(") + name + ") unexpected exception. Execution aborted.");
                    throw;
                }
                _console_ IndentLevel--;
            }
        };

        static bool HasHigherPriority(base const* left, base const* right){
            if(left->isLowPriority != right->isLowPriority)
                return right->isLowPriority;
            String leftName = typeid(*left).name();
            String rightName = typeid(*right).name();
            return leftName < rightName;
        }
    };

    TestFixture::base* TestFixture::currentTest = {};
}