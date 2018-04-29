#ifndef U_PTR_H_
#define U_PTR_H_

#include <typeinfo>

namespace perf {

    #define _typeid_(x) typeid(x).hash_code()
    #define _addr_(x) static_cast<void*>(&x)

    template <typename T, typename U>
    struct Types {
        static constexpr bool ARE_SAME = false;
    };

    template <typename T>
    struct Types<T, T> {
        static constexpr bool ARE_SAME = true;
    };

    template <typename T>
    class b_ptr {

    public:

        virtual void setRaw(T* addr) = 0;

        virtual T* getRaw() const = 0;

        virtual void setType(const size_t& type) = 0;

        virtual size_t getType() const = 0;

    };

    #ifdef PORT_DLL
    template <typename T>
    class PORT_DLL u_ptr : public b_ptr<T>
    #else
    template <typename T>
    class u_ptr : public b_ptr<T>
    #endif
    {

    public:

        u_ptr() : 
            _type(_typeid_(T)), 
            _raw(nullptr) {
                if constexpr (!Types<void, T>::ARE_SAME) {
                    _raw = new T();
                }
            }

        u_ptr(T* ptr) :
            _type(_typeid_(T)),
            _raw(ptr) {}

        template <typename Arg1, typename... Args>
        explicit u_ptr(Arg1&& arg1, Args&&... args) : 
            _type(_typeid_(T)),
            _raw(new T(arg1, args...)) {}


        ~u_ptr() {
            clean<T>();
        }

        u_ptr(const u_ptr&) = delete;

        u_ptr& operator= (const u_ptr& other) = delete;

        template<typename U>
        u_ptr(u_ptr<U>&& other) {
            move<U>(other);
        }

        template<typename U>
        u_ptr& operator= (U* other) {
            clean<T>();
            _raw = other;
            _type = _typeid_(U);
            return *this;
        }
        
        template<typename U>
        u_ptr& operator= (u_ptr<U>&& other) {
            move<U>(other);
            return *this;
        }

        template <typename U = T>
        U& operator* () {
            return *_raw;
        }

        T* operator-> () {
            return _raw;
        }

        bool operator! () const {
            return !_raw;
        }

        template<typename U>
        bool operator== (const u_ptr<U>& other) const {
            return _raw == other.get();
        }

        template<typename U>
        bool operator== (const U* addr) const {
            return _raw == addr;
        }

        template<typename U>
        bool operator!= (const u_ptr<U>& other) const {
            return _raw != other.get();
        }

        template<typename U>
        bool operator!= (const U* addr) const {
            return _raw != addr;
        }

        inline T* get() const {
            return getRaw();
        }

    private:

        template<typename U>
        void clean() {
            if (_raw) {
                if (_type != _typeid_(void)) {
                    delete _raw;
                }
                _raw = nullptr;
            }
        }

        template<typename U>
        void move(b_ptr<U>& other) {
            if (_addr_(*this) != _addr_(other)) {
                clean<T>();
                _raw = other.getRaw();
                _type = other.getType();
                other.setRaw(nullptr);
            }
        }

        virtual T* getRaw() const override {
            return _raw;
        }

        virtual void setRaw(T* addr) override {
            _raw = addr;
        }

        virtual size_t getType() const override {
            return _type;
        }

        virtual void setType(const size_t& type) override {
            _type = type;
        }

    private:
        T* _raw;
        size_t _type;
    };

}
#undef _addr_
#undef _typeid_
#endif