#include <gmock/gmock.h>
#include <vld.h>

int main(int argc, char** argv) {
    VLDGlobalDisable();
<<<<<<< HEAD
    ::testing::InitGoogleMock(&argc,argv);
    RUN_ALL_TESTS();
=======
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
>>>>>>> 631685d... PROJ-Timer added mem-leak guards to gtests. implemented custom unique pointer - used in Timer class.
}