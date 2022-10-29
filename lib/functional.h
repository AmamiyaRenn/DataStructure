#pragma once

#include <new>
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

template<typename Ret, typename... Types>
class FunctionHelperBase
{
public:
    virtual Ret operator()(const Types&... args) = 0;
};

template<typename Func, typename Ret, typename... Types>
class FunctionHelper : public FunctionHelperBase<Ret, Types...>
{
public:
    explicit FunctionHelper(Func functor) : functor(functor) {}
    virtual Ret operator()(const Types&... args) { return functor(args...); }

private:
    Func functor;
};

template<typename>
class Function;

template<typename Ret, typename... Types>
class Function<Ret(Types...)>
{
public:
    Function() = default;
    template<typename Func>
    explicit Function(Func functor) : func_ptr(new FunctionHelper<Func, Ret, Types...>(functor))
    {}
    Ret operator()(const Types&... args) { return (*func_ptr)(args...); }

private:
    FunctionHelperBase<Ret, Types...>* func_ptr;
};