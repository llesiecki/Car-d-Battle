#pragma once

template <typename T> T& Singleton()
{
    static T single;
    return single;
}

template <typename T> T* pSingleton()
{
    return &Singleton<T>();
}
