#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/MyClass.h"

#define LEAK_CHECK 1
#if LEAK_CHECK 
	#include <vld.h>
#endif

/*
TEST(keon, keon) {
	ASSERT_TRUE(true);
}
*/

template <typename T>
void printVector(const std::vector<T>& v) {

	for (auto e : v) {
		std::cout << e << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	
	/*
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
	*/

	MyClass<int> mc = MyClass<int>::MyClassFactory::factory()
		.add(1)
		.add(2)
		.add(3)
		.add(4)
		.add(5)
		.add(6)
		.add(7)
		.build()
		;

	printVector(mc.getVector());
	return 0;
}