#ifndef MYCLASS_H
#define MYCLASS_H

#include <vector>
#include <memory>

#define DEBUG 1

#if DEBUG
	#include <iostream>
#endif


template <typename T>
class MyClass {

	private:
		std::vector<T> m_v;	
		bool m_isValidated;
	
	public:
		MyClass();
		MyClass(const MyClass& other);
		~MyClass();
		const std::vector<T>& getVector();

		class MyClassFactory {

			private:
				MyClass* m_myClass;
				
			public:
				MyClassFactory(MyClass* mc);
				MyClassFactory(const MyClassFactory& other);
				MyClassFactory(const MyClassFactory&& other);
				~MyClassFactory();
				static MyClassFactory factory();
				MyClassFactory& add(const T& val);
				MyClass* build();

		};
};

#include "MyClass.hpp"

#endif
