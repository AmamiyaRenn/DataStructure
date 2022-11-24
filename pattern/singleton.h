#pragma once

template<typename T>
class Singleton
{
public:
    Singleton()                            = delete;
    Singleton(const Singleton&)            = delete;
    Singleton(Singleton&&)                 = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&)      = delete;

    static T* instance()
    {
        if (m_instance == nullptr)
            m_instance = new T;
        return m_instance;
    }

protected:
    static T* m_instance;
};

template<typename T>
T* Singleton<T>::m_instance = nullptr;