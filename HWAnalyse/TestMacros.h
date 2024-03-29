#pragma once
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/vmd/is_empty.hpp>
#include <boost/vmd/to_seq.hpp>

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

#define HW_TM_IMPLEMENTATION_I(name,initialize)                                                      \
    HW_TM_CLASS_NAME(name)::HW_TM_CLASS_NAME(name)(): Data(initialize){};                               \
    string HW_TM_CLASS_NAME(name)::location()const {return HW_FILE_LINE_FUNCTION;};                  \
    HWLib::RuntimeChain<HWAnalysis::TestFixture> HW_TM_INSTANCE_NAME(name) = new HW_TM_CLASS_NAME(name)(); \
    void HW_TM_CLASS_NAME(name)::Run() const

#define HW_TM_DEPENDENCY_INITIALIZE_I_BOOST(r,d,i,name) (HW_TM_INSTANCE_NAME(name).data)
#define HW_TM_DEPENDENCY_INITIALIZE_I_NOT_EMPTY(...)BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH_I(HW_TM_DEPENDENCY_INITIALIZE_I_BOOST, ~, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))
#define HW_TM_DEPENDENCY_INITIALIZE_I_HW(...)BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH_I(HW_TM_DEPENDENCY_INITIALIZE_I_BOOST, ~, HW_VARIADIC_TO_SEC(__VA_ARGS__)))

#define HW_TM_DEPENDENCY_INITIALIZE_I(...)BOOST_PP_IF(BOOST_VMD_IS_EMPTY(__VA_ARGS__),BOOST_VMD_EMPTY,HW_TM_DEPENDENCY_INITIALIZE_I_NOT_EMPTY)(__VA_ARGS__)
#define HW_TM_IMPLEMENTATION(name,...) HW_TM_IMPLEMENTATION_I(name,{HW_TM_DEPENDENCY_INITIALIZE_I(__VA_ARGS__)})
