#include "MyClass.h"

template <typename T>
MyClass<T>::MyClass() {
#if DEBUG
	std::cout << "Class Constructor called.\n";
#endif
	this->m_v.reserve(5);
}

template<typename T>
MyClass<T>::MyClass(const MyClass & other)
{
#if DEBUG
	std::cout << "Class Copy Constructor called.\n";
#endif
	this->m_isValidated = other.m_isValidated;
	this->m_v = other.m_v;
}

template<typename T>
MyClass<T>::MyClass(MyClass&& other)
{
#if DEBUG
	std::cout << "Class Move Constructor called.\n";
#endif
		this->m_isValidated = other.m_isValidated;
		this->m_v = other.m_v;
}

template<typename T>
MyClass<T>& MyClass<T>::operator=(const MyClass &other)
{

#if DEBUG
	std::cout << "Class Copy Assignment Operator called.\n";
#endif
	if (this != &other) {
		this->m_isValidated = other.m_isValidated;
		this->m_v = other.m_v;
	}
	return *this;
}

template<typename T>
MyClass<T>& MyClass<T>::operator= (MyClass&& other)
{
#if DEBUG
	std::cout << "Class Move Assignment Operator called.\n";
#endif		
	this->m_isValidated = other.m_isValidated;
	this->m_v = other.m_v;
	return *this;
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

template<typename T>
MyClass<T>::MyClassFactory::MyClassFactory(){
}

template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(const MyClass<T>& mc) {
#if DEBUG
	std::cout << "Factory Class Constructor called.\n";
#endif
	m_myClass = std::move(mc);
}

template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(const MyClassFactory& other) {
#if DEBUG
	std::cout << "Factory Class Copy Constructor called.\n";
#endif
	m_myClass = new MyClass();
	*m_myClass = *other.m_myClass;
}



template <typename T>
MyClass<T>::MyClassFactory::MyClassFactory(MyClass<T>::MyClassFactory&& other) {
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
typename MyClass<T>::MyClassFactory MyClass<T>::MyClassFactory::factory() {
	return std::move(*std::make_unique<MyClass<T>::MyClassFactory>(MyClass()));
}

template <typename T>
typename const MyClass<T>& MyClass<T>::MyClassFactory::build() {
	m_myClass.m_isValidated = true;	
	return m_myClass;
}

template <typename T>
typename MyClass<T>::MyClassFactory& MyClass<T>::MyClassFactory::add(const T& val) {
	m_myClass.m_v.emplace_back(val);
#if DEBUG
	std::cout << "Added: " << val << std::endl;
#endif
	return *this;
}

