#pragma once
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#define test_class_name(name) name 
#define test_instance_name(name) _##name 
#define test_instance(name) HWLib::RuntimeChain<HWAnalyse::TestFixture> test_instance_name(name)(new test_class_name(name)())
#define test_implementation(name) void test_class_name(name)::Run() const
#define test_(name) test_class(name,{}); test_instance(name); test_implementation(name)
#define test_item(name) (*test_instance_name(name).data)

#define test_class(name,initialize) \
struct test_class_name(name) final: public HWAnalyse::TestFixture::base{\
    test_class_name(name)(): base(initialize){}; \
    String const location()const override {return FILE_LINE_FUNCTION;};\
    void Run() const override;\
}

#define TEST_DEPENDENY_INITIALIZE1(r,d,i,name) (test_instance_name(name).data)
#define TEST_DEPENDENY_INITIALIZE(...) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH_I(TEST_DEPENDENY_INITIALIZE1, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define test(name,...) \
    test_class(name,{TEST_DEPENDENY_INITIALIZE(__VA_ARGS__)}); \
    test_instance(name); \
    test_implementation(name)

