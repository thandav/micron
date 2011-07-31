#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{
    public:
        static Singleton<T>& getInstance();
    protected:
    private:
        Singleton<T>();
        ~Singleton<T>();
        Singleton<T>(const Singleton<T>& other);
        Singleton<T>& operator=(const Singleton<T>& other);
};

#endif // SINGLETON_H
