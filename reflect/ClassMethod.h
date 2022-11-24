#pragma once

#include <string>
#include <vadefs.h>

class ClassMethod
{
public:
    ClassMethod(const std::string& name, uintptr_t method) : m_name(name), m_method(method) {}

    const std::string& name() const { return m_name; }
    uintptr_t          method() const { return m_method; }

private:
    std::string m_name;
    uintptr_t   m_method;
};
