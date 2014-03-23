#pragma once
#define test_class_name(name) name 
#define test_instance_name(name) _##name 
#define test_class(name) \
struct test_class_name(name) final: public HWAnalyse::TestFixture::base{\
    test_class_name(name)(): base(){}; \
    String const location()const override {return FILE_LINE_FUNCTION;}\
    void Run() const override;\
}
#define test_instance(name) HWLib::RuntimeChain<HWAnalyse::TestFixture> test_instance_name(name)(new test_class_name(name)());
#define test_implementation(name) void test_class_name(name)::Run() const
#define test(name) test_class(name); test_instance(name); test_implementation(name)
#define test_item(name) (*test_instance_name(FileTest).data)
