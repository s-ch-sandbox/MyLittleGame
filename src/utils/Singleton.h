#pragma once
#include <assert.h>

#include <memory>

namespace utils {
template <class T>
class Singleton {
   public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T* GetInstance() {
        assert(instance_);
        return instance_.get();
    }

    template <typename... TArgs>
    static void InitInstance(TArgs&&... args) {
        assert(!instance_);
        instance_.reset(new T(std::forward<TArgs>(args)...));
    }

    static void InitInstance() {
        assert(!instance_);
        instance_.reset(new T);
    }

    static void DestroyInstance() {
        assert(instance_);
        instance_.reset();
    }

   protected:
    Singleton() = default;
    static std::unique_ptr<T> instance_;
};

template <class T>
std::unique_ptr<T> Singleton<T>::instance_;
}  // namespace utils
