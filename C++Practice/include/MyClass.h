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
	std::vector<T> m_v;
	
	bool m_isValidated;

	MyClass() : m_factory(nullptr){}

public:

	MyClass(const MyClass& other) = delete;

	~MyClass() { 
		delete m_factory; 
	}

	const std::vector<T>& getVector();

	class MyClassFactory {

	private:
		MyClass* m_myClass;

		MyClassFactory(MyClass* mc) : m_myClass(mc) {
			m_myClass->m_factory = this; 
			#if DEBUG
				std::cout << "Factory Class Constructor\n";
			#endif
		}
		

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
