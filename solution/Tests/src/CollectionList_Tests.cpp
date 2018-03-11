#include "gtest-utilities.h"
#include <string>
#include "CollectionList.h"
#if 1
class CollectionFixture : public ::testing::Test {
};

TEST_F(CollectionFixture, push_popFront_Test) {	
    run([](){
        #if 1
        CollectionList collection;
	    collection.push_front<int>(5);
	    collection.push_front<float>(5.0f);
        
	    collection.push_front<std::string>("s1");
	    collection.push_front<std::string>("s2");
	    collection.push_front<std::string>("s3");
	    _NO_LEAK_CHECK(ASSERT_EQ("s3", collection.pop_front<std::string>()));
	    _NO_LEAK_CHECK(ASSERT_EQ("s2", collection.pop_front<std::string>()));
	    _NO_LEAK_CHECK(ASSERT_EQ("s1", collection.pop_front<std::string>()));
        
	    _NO_LEAK_CHECK(ASSERT_EQ(5.0f, collection.pop_front<float>()));
	    _NO_LEAK_CHECK(ASSERT_EQ(5, collection.pop_front<int>()));
        #endif
    });
}

TEST_F(CollectionFixture, iteratorEmptyCollection_ThrowsException_Test) {
    run([](){
        #if 0
        CollectionList collection;
	    auto iter = collection.front();
	    _NO_LEAK_CHECK(ASSERT_THROW(iter.get<int>(), EmptyCollectionException));
        #endif
    });
}

TEST_F(CollectionFixture, iteratorNonEmptyCollection_getElement_Test) {
    run([](){
        #if 0
        CollectionList collection;
	    collection.push_front<int>(3);
	    collection.push_front<char>('c');
	    auto iter = collection.front();
	    _NO_LEAK_CHECK(ASSERT_EQ('c', iter.get<char>()));
	    --iter;
	    _NO_LEAK_CHECK(ASSERT_EQ(3, iter.get<int>()));
        #endif
    });
}
#endif