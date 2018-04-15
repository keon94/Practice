#include <gmock/gmock.h>
//#include <vld.h>

int main(int argc, char** argv) {
	::testing::InitGoogleMock(&argc, argv);
	std::string s, u;
	s == u;
	return RUN_ALL_TESTS();
}