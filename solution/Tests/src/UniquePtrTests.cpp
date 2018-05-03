#include "gtest-utilities.h"
#include "u_ptr.h"
#include <vld.h>

#define DISPLAY_LEAKS_DETAILS 1

class UptrFixture : public testing::Test {

protected:

    struct X {
        virtual int returnOne() = 0;
        virtual ~X() {}
    };

    struct Y : public X {
        char* str = new char[10];
        ~Y() {
            delete[] str;
        }
        virtual int returnOne() {
            return 1;
        }

        int returnTwo() {
            return 2;
        }
    };
};

using perf::u_ptr;

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
    int x;
    run([&]() {
        u_ptr<void> vptr1 = u_ptr<int>(7); 
        u_ptr<int> intptr = std::move(vptr1);
        u_ptr<void> vptr2 = std::move(intptr);
        x = *static_cast<int*>(vptr2);
    });
    ASSERT_EQ(x, 7);
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

TEST_F(UptrFixture, type_casts) {
    run([&]() {
        auto u = u_ptr<int>(5);
        void* p_v = static_cast<void*>(u);
        int* p_d = static_cast<int*>(p_v);
        int x0 = *p_d;
        _NO_LEAK_CHECK(ASSERT_EQ(x0, 5));
    });
}

TEST_F(UptrFixture, arr_ptr) {
    int x0, x1, x2;
    run([&]() {
        u_ptr<int> ptr = u_ptr<int[3]>();
        ptr.get()[0] = 0;
        ptr.get()[1] = 1;
        ptr.get()[2] = 2;
        int* p = ptr;
        x0 = *p;
        x1 = *++p;
        x2 = *++p;
    });
    ASSERT_EQ(x0, 0);
    ASSERT_EQ(x1, 1);
    ASSERT_EQ(x2, 2);
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
    run([&]() {
        u_ptr<X> xPtr = u_ptr<Y>(new Y());
        int x = xPtr->returnOne();
        _NO_LEAK_CHECK(ASSERT_EQ(x, 1));
        Y* yPtr = dynamic_cast<Y*>(xPtr.get());
        int y = yPtr->returnTwo();
        _NO_LEAK_CHECK(ASSERT_EQ(y, 2));
    });
}