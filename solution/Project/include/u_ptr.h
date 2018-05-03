#ifndef U_PTR_H_
#define U_PTR_H_

#ifndef PORT_DLL
#define PORT_DLL
#endif

#define _addr_(x) static_cast<void*>(&x)

namespace perf {

    template <typename T, typename U>
    struct Types {
        static constexpr bool ARE_SAME = false;
    };

    template <typename T>
    struct Types<T, T> {
        static constexpr bool ARE_SAME = true;
    };

    template<typename T>
    struct IsArray {
        static constexpr bool VALUE = false;
    };

    template<typename T>
    struct IsArray<T[]> {
        static constexpr bool VALUE = true;
    };

    template<typename T, unsigned N>
    struct IsArray<T[N]> {
        static constexpr bool VALUE = true;
    };

    template <typename T>
    using Deleter = void(*)(T*);

    template <typename T>
    class b_ptr {

    public:

        virtual void setRaw(T* addr) = 0;

        virtual T* getRaw() const = 0;

    };

    template <typename T>
    class PORT_DLL u_ptr : public b_ptr<T>
    {

    public:

        u_ptr() :
            _deleter(setDefaultDeleter()),
            _raw(nullptr)
        {
            if constexpr (IsArray<T>::VALUE && !Types<void, T>::ARE_SAME) {
                _raw = new T();
            }
        }

        u_ptr(T* ptr) :
            _deleter(setDefaultDeleter()),
            _raw(ptr) {}

        template <typename ... Args> //new T(args...)
        explicit u_ptr(Args&&... args) :
            _deleter(setDefaultDeleter()),
            _raw(new T(args...)) {}

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
            move<U>(other);
            return *this;
        }

        template<typename U>
        u_ptr& operator= (u_ptr<U>&& other) {
            move<U>(other);
            return *this;
        }

        template<typename U>
        U& operator* () {
            return *_raw;
        }

        T* operator-> () {
            return static_cast<T*>(_raw);
        }

        bool operator! () const {
            return !_raw;
        }

        template<typename U>
        operator U* () const {
            return reinterpret_cast<U*>(_raw);
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

        inline u_ptr& setDeleter(const Deleter<T>& deleter) {
            _deleter = deleter;
            return *this;
        }

        template <typename U>
        inline Deleter<U> getDeleter() const {
            if constexpr (!Types<U, T>::ARE_SAME)
                return [](U* addr) { delete addr; };
            else
                return _deleter;
        }

    private:

        template<typename U>
        void clean() {
            if (_raw) {
                _deleter(static_cast<U*>(_raw));
                _raw = nullptr;
            }
        }

        template<typename U>
        void move(u_ptr<U>& other) {
            if (_addr_(*this) != _addr_(other)) {
                clean<T>();
                _raw = static_cast<b_ptr<U>&>(other).getRaw();
                if constexpr (!Types<void, U>::ARE_SAME)
                    _deleter = other.getDeleter<T>();
                static_cast<b_ptr<U>&>(other).setRaw(nullptr);
            }
        }

        template<typename U>
        void move(U* addr) {
            clean<T>();
            _raw = addr;
            if constexpr (!Types<void, U>::ARE_SAME)
                _deleter = [](U* addr) { delete addr; };
        }

        virtual T* getRaw() const override {
            return static_cast<T*>(_raw);
        }

        virtual void setRaw(T* addr) override {
            _raw = addr;
        }

        inline Deleter<T> setDefaultDeleter() {
            if constexpr (IsArray<T>::VALUE) {
                return [](T* addr) { return delete[] addr; };
            }
            else {
                return [](T* addr) { return delete addr; };
            }
        }

    private:
        void* _raw;
        Deleter<T> _deleter;
    };

}
#undef _addr_
#endif