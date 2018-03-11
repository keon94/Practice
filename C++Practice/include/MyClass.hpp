#include "MyClass.h"

template <typename T>
MyClass<T>::MyClass() {
#if DEBUG
	std::cout << "Class Destructor called.\n";
#endif
}

template<typename T>
MyClass<T>::MyClass(const MyClass & other)
{
#if DEBUG
	std::cout << "Class Copy Constructor called.\n";
#endif
}


template <typename T>
MyClass<T>::~MyClass() {
#if DEBUG
	std::cout << "Class Destructor called.\n";
#endif
}

template <typename T>
typename const std::vector<T>& MyClass<T>::getVector() {
	return m_v;
}


//.......FACTORY...........

template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(MyClass<T>* mc) : m_myClass(mc) {
#if DEBUG
	std::cout << "Factory Class Constructor called.\n";
#endif
}

template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(const MyClass<T>::MyClassFactory& other)  : m_myClass(other.m_myClass){
#if DEBUG
	std::cout << "Factory Class Copy Constructor called.\n";
#endif
}

template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(const MyClass<T>::MyClassFactory&& other) {
#if DEBUG
	std::cout << "Factory Class Move Constructor called.\n";
#endif
	m_myClass = std::move(other.m_myClass);
}

template<typename T>
MyClass<T>::MyClassFactory::~MyClassFactory() {
#if DEBUG
	std::cout << "Factory Class Destructor called.\n";
#endif
}

template <typename T>
//typename MyClass<T>::MyClassFactory MyClass<T>::MyClassFactory::factory() {
typename MyClass<T>::MyClassFactory MyClass<T>::MyClassFactory::factory() {
	std::unique_ptr<MyClass<T>::MyClassFactory> s = std::make_unique<MyClass<T>::MyClassFactory>(new MyClass());
	return std::move(*s);
}

template <typename T>
typename MyClass<T>* MyClass<T>::MyClassFactory::build() {
	m_myClass->m_isValidated = true;	
	return m_myClass;
}

template <typename T>
typename MyClass<T>::MyClassFactory& MyClass<T>::MyClassFactory::add(const T& val) {
	m_myClass->m_v.emplace_back(val);
	return *this;
}

