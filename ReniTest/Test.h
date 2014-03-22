#pragma once
#include "../HWLib/String.h"
#include "../HWLib/RuntimeChain.h"

#define test_class_name(name) name 
#define test_instance_name(name) _##name 
#define test_class(name) \
struct test_class_name(name) final: public HWTest::TestFixture::base{\
    void Run() const override;\
}
#define test_instance(name) HWLib::RuntimeChain<HWTest::TestFixture> test_instance_name(name)(new test_class_name(name) ());
#define test_implementation(name) void test_class_name(name)::Run() const
#define test(name,...) test_class(name); test_instance(name); test_implementation(name)

using namespace HWLib;


namespace HWTest{
    struct TestFixture{
        struct base{
            Array<CtrlRef<base>> dependencies;
            virtual void Run()const = 0;
        };

        typedef base const* dataType;

        static void RunAll(){
            auto all = RuntimeChain<TestFixture>::All()
                ->Sort(HasHigherPriority)
                ->ToArray;
            for(int i = 0; i < all.Count; i++){
                auto& t = *all[i];
                auto name = typeid(t).name();
                t.Run();
            }
        };

        static bool HasHigherPriority(base const* left, base const* right){
            auto leftName = typeid(*left).name();
            auto rightName = typeid(*right).name();
            while(true){
                if(!leftName)
                    return true;
                if(!rightName)
                    return false;
                if(*leftName < *rightName)
                    return true;
                if(*leftName > *rightName)
                    return false;
                leftName++;
                rightName++;
            }
        }
    };
}