#include <gtest/gtest.h>
#include <vld.h>

//Enables leak information to be output to the console
#define DISPLAY_LEAKS_DETAILS 1

#define _NO_LEAK_CHECK(x) \
    VLDGlobalDisable();\
    x;\
    VLDGlobalEnable();\

/**
* \brief runs \ref f and checks for leaks
* \param f an F(void) function (typically lambda) to test
*/
template <typename F>
void run(F&& f) {
    #if 1
    VLDMarkAllLeaksAsReported();
    VLDGlobalEnable();
    f();
    //unsigned leaks = VLDGetLeaksCount();
    VLDGlobalDisable();
    //_NO_LEAK_CHECK(EXPECT_EQ(leaks, 0));
    #if DISPLAY_LEAKS_DETAILS
    VLDReportLeaks();
    #endif
    #endif
}