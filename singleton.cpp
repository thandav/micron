#include "singleton.h"

#include <iostream>

using namespace std;

template <class T>
Singleton::Singleton()
{
    cout << "Singleton constructor is called" << endl;
}

Singleton<T>::~Singleton()
{
    cout << "Singleton destructor is called" << endl;
}

T& Singleton<T>::getInstance()
{
    static T s;
    return s;
}
