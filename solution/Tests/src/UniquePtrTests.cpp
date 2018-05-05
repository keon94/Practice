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

using util::u_ptr;

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
    std::string x0, x1, x2;
    run([&]() {
        u_ptr<std::string> ptr = u_ptr<std::string[3]>();
        ptr.get()[0] = "str0";
        ptr.get()[1] = "str1";
        ptr.get()[2] = "str2";
        std::string* p = ptr;
        x0 = *p;
        x1 = *++p;
        x2 = *++p;
    });
    ASSERT_EQ(x0, "str0");
    ASSERT_EQ(x1, "str1");
    ASSERT_EQ(x2, "str2");
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

TEST_F(UptrFixture, nested_ptrs) {
    run([&]() {
        u_ptr<u_ptr<std::string>> ptr2 = u_ptr<u_ptr<std::string>>("keon");
        u_ptr<std::string> ptr = std::move(*ptr2.get());
        std::string str = *ptr.get();
        _NO_LEAK_CHECK(ASSERT_EQ(str, "keon"));
    });
}

TEST_F(UptrFixture, arr_of_ptrs) {
    #if 1
    run([&]() {
        const unsigned xSize = 4, ySize = 3;
        u_ptr<u_ptr<std::string>> ptr2 = u_ptr<u_ptr<std::string[xSize]>[ySize]>();
        std::string s;
        unsigned count = 0;
        for(unsigned ix = 0; ix < xSize; ++ix) {
            for(unsigned iy = 0; iy < ySize; ++iy) {
                ptr2.get()[iy].get()[ix] = s = 'A' + count;
                ++count;
            }
        }
        count = 0;
        for(unsigned ix = 0; ix < xSize; ++ix) {
            for(unsigned iy = 0; iy < ySize; ++iy) {
                _NO_LEAK_CHECK(ASSERT_EQ(ptr2.get()[iy].get()[ix], s = 'A' + count));
                ++count;
            }
        }
    });
    #endif
}

TEST_F(UptrFixture, scope_change) {
    run([&]() {
        auto f = [](u_ptr<std::string>& uptr) {
            std::string s = *uptr.get();
            return std::move(uptr);
        };
        u_ptr<std::string> ptr = u_ptr<std::string>("keon");
        auto retptr = f(ptr);
        _NO_LEAK_CHECK(ASSERT_EQ(*retptr.get(), "keon"));
    });    
}