#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T, typename D = T>
class Singleton
{
    friend D;
//    static_assert(std::is_base_of_v<T, D>, "T should be a base type for D");

public:
    static T& instance();

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton( const Singleton& ) = delete;
    Singleton& operator=( const Singleton& ) = delete;
};

template <typename T, typename D>
T& Singleton<T, D>::instance()
{
    static D inst;
    return inst;
}

#endif // SINGLETON_H
