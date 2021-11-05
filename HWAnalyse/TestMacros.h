#pragma once
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#define HW_TM_CLASS_NAME(name) name
#define HW_TM_INSTANCE_NAME(name) HW_TM_CLASS_NAME(name)::instance
#define HW_TM_ITEM(name) (*HW_TM_INSTANCE_NAME(name).data)

#define HW_TM_CLASS(name)                                             \
struct HW_TM_CLASS_NAME(name) final: public HWAnalysis::TestFixture::Data{\
    HW_TM_CLASS_NAME(name)();                                           \
    string location()const override;                            \
    void Run() const override;                                       \
    static HWLib::RuntimeChain<HWAnalysis::TestFixture> instance;                          \
}

#define HW_TM_IMPLEMENTATION(name,initialize)                                                      \
    HW_TM_CLASS_NAME(name)::HW_TM_CLASS_NAME(name)(): Data(initialize){};                               \
    string HW_TM_CLASS_NAME(name)::location()const {return FILE_LINE_FUNCTION;};                  \
    HWLib::RuntimeChain<HWAnalysis::TestFixture> HW_TM_INSTANCE_NAME(name) = new HW_TM_CLASS_NAME(name)(); \
    void HW_TM_CLASS_NAME(name)::Run() const

#define HW_TM_DEPENDENCY_INITIALIZE_I(r,d,i,name) (HW_TM_INSTANCE_NAME(name).data)
#define HW_TM_DEPENDENCY_INITIALIZE(...) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH_I(HW_TM_DEPENDENCY_INITIALIZE_I, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))

#define HW_TM_TEST_(name) HW_TM_IMPLEMENTATION(name,{})
#define HW_TM_TEST(name,...) HW_TM_IMPLEMENTATION(name,{HW_TM_DEPENDENCY_INITIALIZE(__VA_ARGS__)})

