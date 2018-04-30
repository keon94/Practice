#include <gtest/gtest.h>
#include <vld.h>

//Enables leak information to be output to the console
#define DISPLAY_LEAKS_DETAILS 1

/**
 * \brief runs \ref f and checks for leaks
 * \param f an F(void) function (typically lambda) to test 
 */
template <typename F>
void test_with_leak_check(F&& f) {
    VLDMarkAllLeaksAsReported();
    VLDGlobalEnable();
    f();
    VLDGlobalDisable();
    unsigned leaks = VLDGetLeaksCount();
    EXPECT_EQ(leaks, 0);
    #if DISPLAY_LEAKS_DETAILS
    VLDReportLeaks();
    #endif
}