
#include <iostream>
#include <vector>

template <typename T>
class Base {

public:

	const std::vector<T>& getVector();

	template <typename SubBaseFactory>
	class BaseFactory {

	public:
		static SubBaseFactory* factory();
		Base* build();

	protected:
		std::vector<T>& getVector();

	private:
		BaseFactory();
		Base * m_Base;

	};

private:
	std::vector<T> m_vector;

};

template<typename T>
const std::vector<T>& Base<T>::getVector()
{
	return this->m_vector;
}

class DerivedFactory : public Base<int>::BaseFactory<DerivedFactory> {
public:
	DerivedFactory * add(int);

};


template<typename T, typename SubBaseFactory>
using BaseFactory = Base<T>::BaseFactory<SubBaseFactory>;


template<typename T>
template<typename SubBaseFactory>
SubBaseFactory* BaseFactory<T, SubBaseFactory>::factory()
{
	return static_cast<SubBaseFactory*>(new BaseFactory());
}


template<typename T>
template<typename SubBaseFactory>
Base<T>* BaseFactory<T, SubBaseFactory>::build()
{
	return m_Base;
}

template<typename T>
template<typename SubBaseFactory>
std::vector<T>& BaseFactory<T, SubBaseFactory>::getVector()
{
	return this->m_Base->m_vector;
}

template<typename T>
template<typename SubBaseFactory>
BaseFactory<T, SubBaseFactory>::BaseFactory()
{
	m_Base = new Base();
}

DerivedFactory * DerivedFactory::add(int x)
{
	getVector().emplace_back(x);
	return this;
}

template <typename T>
void printVector(const std::vector<T> v) {
	for (auto e : v)
		std::cout << e << " ";
	std::cout << std::endl;
}


int main() {

	auto b = DerivedFactory::factory()
		->add(1)
		->add(2)
		->add(3)
		->build();
	printVector(b->getVector());
	return 0;
}


