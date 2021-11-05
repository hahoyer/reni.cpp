#pragma once
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#define test_class_name(name) name 
#define test_instance_name(name) name::instance 
#define test_item(name) (*test_instance_name(name).data)

#define test_class(name)                                             \
struct test_class_name(name) final: public HWAnalysis::TestFixture::Data{\
    test_class_name(name)();                                           \
    string location()const override;                            \
    void Run() const override;                                       \
    static HWLib::RuntimeChain<HWAnalysis::TestFixture> instance;                          \
}

#define test_implementation(name,initialize)                                                      \
    test_class_name(name)::test_class_name(name)(): Data(initialize){};                               \
    string test_class_name(name)::location()const {return FILE_LINE_FUNCTION;};                  \
    HWLib::RuntimeChain<HWAnalysis::TestFixture> test_instance_name(name) = new test_class_name(name)(); \
    void test_class_name(name)::Run() const

#define TEST_DEPENDENY_INITIALIZE1(r,d,i,name) (test_instance_name(name).data)
#define TEST_DEPENDENY_INITIALIZE(...) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH_I(TEST_DEPENDENY_INITIALIZE1, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define test_(name) test_implementation(name,{})
#define test(name,...) test_implementation(name,{TEST_DEPENDENY_INITIALIZE(__VA_ARGS__)})

