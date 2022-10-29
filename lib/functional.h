#pragma once

template<typename Tp>
struct RemoveReference
{
    using type = Tp;
};

template<typename Tp>
struct RemoveReference<Tp&>
{
    using type = Tp;
};

template<typename Tp>
struct RemoveReference<Tp&&>
{
    using type = Tp;
};

// 左值完美转发
template<class T>
inline T&& forward(typename RemoveReference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

// 右值完美转发
template<class T>
inline T&& forward(typename RemoveReference<T>::type&& t) noexcept
{
    // static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}