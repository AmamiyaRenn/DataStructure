#pragma once

#include <string>

class ClassField
{
public:
    ClassField() = default;
    ClassField(const std::string& name, const std::string& type, size_t m_offset) :
        m_name(name), m_type(type), m_offset(m_offset)
    {}

    const std::string& name() const { return m_name; }
    const std::string& type() const { return m_type; }
    size_t             offset() const { return m_offset; }

private:
    std::string m_name;
    std::string m_type;
    size_t      m_offset {0};
};
