#include <gtest/gtest.h>
#include "u_ptr.h"
#include <vld.h>

#define DISPLAY_LEAKS_DETAILS 1

class UptrFixture : public testing::Test {

protected:

    struct X {
        virtual int returnOne() = 0;
    };

    struct Y : public X {
        virtual int returnOne() {
            return 1;
        }
    };
};

using perf::u_ptr;

template <typename F>
void run(F&& f) {
    VLDMarkAllLeaksAsReported();
    VLDGlobalEnable();
    f();
    unsigned leakCount = VLDGetLeaksCount();
    VLDGlobalDisable();
    EXPECT_EQ(leakCount, 0);
    #if DISPLAY_LEAKS_DETAILS
    VLDReportLeaks();
    #endif
}

TEST_F(UptrFixture, void_to_int_moveAssignment) {
    int x;
    run([&]() {
        u_ptr<void> ptr;
        ptr = u_ptr<int>(7);
        x = *static_cast<int*>(ptr.get());
    });
    ASSERT_EQ(x, 7); 
}

TEST_F(UptrFixture, void_to_int_to_void_moveAssignment) {

}

TEST_F(UptrFixture, void_to_int_moveConstructor) {
    int x;
    run([&]() {
        u_ptr<void> ptr;
        ptr = std::move(u_ptr<int>(7));
        x = *static_cast<int*>(ptr.get());
    });
    ASSERT_EQ(x, 7);
}

TEST_F(UptrFixture, no_mem_leak_void_ptr) {

}

TEST_F(UptrFixture, no_mem_arr_ptr) {
    run([&]() {
        u_ptr<int[]> ptr;
    });
}

TEST_F(UptrFixture, no_mem_leak_int_ptr) {
    run([&]() {
        int* x = new int(1);
        {
            u_ptr<int> ptr = x;
        }
    });
}

TEST_F(UptrFixture, polymorphism) {
    int x;
    run([&]() {
    u_ptr<X> ptr = u_ptr<Y>();
    x = ptr->returnOne();
    });
    ASSERT_EQ(x , 1);
}
