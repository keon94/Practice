#include "MyClass.h"
template <typename T>
MyClass<T>::MyClass() : m_factory(nullptr) {}


template <typename T>
MyClass<T>::~MyClass() {
	delete m_factory;
}

template <typename T>
typename const std::vector<T>& MyClass<T>::getVector() {
	return m_v;
}


//.......FACTORY...........

template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(MyClass<T> * mc) : m_myClass(mc) {
	m_myClass->m_factory = this;
#if DEBUG
	std::cout << "Factory Class Constructor\n";
#endif
}


template <typename T>
typename MyClass<T>::MyClassFactory* MyClass<T>::MyClassFactory::factory() {
	MyClassFactory* cf = new MyClassFactory(new MyClass());
	return cf;
}

template <typename T>
typename MyClass<T>* MyClass<T>::MyClassFactory::build() {
	m_myClass->m_isValidated = true;
	
	return m_myClass;
}

template <typename T>
typename MyClass<T>::MyClassFactory* MyClass<T>::MyClassFactory::add(const T& val) {
	m_myClass->m_v.emplace_back(val);
	return this;
}

