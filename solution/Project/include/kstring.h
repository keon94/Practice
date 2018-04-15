#include <iosfwd>
#include <string.h>

class Kstring {

public:
	Kstring() : _str(nullptr), _len(0) {}

	Kstring(const char* str) : Kstring() {
		_len = strlen(str);
		_len += (str[_len - 1] == 0 ? 0 : 1);
		_str = static_cast<char*>(std::malloc(_len * sizeof(char)));
		memcpy(_str, str, _len);
		_str[_len - 1] = 0;
	}

	~Kstring() {
		freeStr();
	}

	Kstring(const Kstring& str) {
		_len = str._len;
		_str = _str = static_cast<char*>(std::malloc(_len * sizeof(char)));
		memcpy(_str, str._str, _len);
	}

	Kstring(Kstring&& str) : _str(str._str) {
		str._str = nullptr;
	}

	Kstring& operator= (const char* str) {
		freeStr();
		_len = strlen(str);
		_len += (str[_len - 1] == 0 ? 0 : 1);
		_str = static_cast<char*>(std::malloc(_len * sizeof(char)));
		memcpy(_str, str, _len);
		return *this;
	}

	Kstring& operator= (const Kstring& str) {
		freeStr();
		_len = str._len;
		_str = _str = static_cast<char*>(std::malloc(_len * sizeof(char)));
		memcpy(_str, str._str, _len);
		return *this;
	}

	Kstring& operator= (Kstring&& str) {
		_str = str._str;
		str._str = nullptr;
		return *this;
	}

	bool operator== (const Kstring& str) const {
		if (_len != str._len)
			return false;
		bool areEqual;
		for (size_t i = 0; areEqual = _str[i] == str._str[i] && i != _len; ++i);
		return areEqual;
	}

	bool operator== (const char* str) const {
		if (_len != strlen(str))
			return false;
		bool areEqual;
		for (size_t i = 0; areEqual = _str[i] == str[i] && i != _len; ++i);
		return areEqual;
	}

	bool operator!= (const char* str) const {
		return !(*this != str);
	}

	bool operator!= (const Kstring& str) const {
		return !(*this == str);
	}

	friend std::ostream& operator<< (std::ostream& os, const Kstring& str) {
		os << str._str;
		return os;
	}

	char* to_Cstring() const {
		return _str;
	}

private:
	char* _str;
	size_t _len;

private:
	inline void freeStr() {
		if (_str) {
			std::free(_str);
			_str = nullptr;
		}
	}
};