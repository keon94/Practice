#ifndef MYCLASS_H
#define MYCLASS_H

#include <vector>
#define DEBUG 1

#if DEBUG
	#include <iostream>
#endif


template <typename T>
class MyClass {

private:
	MyClass();
	std::vector<T> m_v;	
	bool m_isValidated;
	

public:

	MyClass(const MyClass& other) = delete;

	~MyClass();

	const std::vector<T>& getVector();

	class MyClassFactory {

	private:
		MyClass* m_myClass;

		MyClassFactory(MyClass* mc);

	public:
		static MyClassFactory* factory();
		MyClassFactory* add(const T& val);
		MyClass* build();
		~MyClassFactory() {}

	};
private:
	MyClassFactory* m_factory;

};

#include "MyClass.hpp"

#endif
