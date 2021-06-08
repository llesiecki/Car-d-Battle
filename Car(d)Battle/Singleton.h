#pragma once

template <typename T> T& Singleton()
{
    static T single;
    return single;
}