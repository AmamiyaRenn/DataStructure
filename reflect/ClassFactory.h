#pragma once

#include "../pattern/singleton.h"
#include "../reflect/ClassField.h"
#include "../reflect/ClassMethod.h"
#include <functional>
#include <map>
#include <stdint.h>
#include <string>
#include <vadefs.h>
#include <vector>


class Object;

using create_object = Object* (*)();

class ClassFactory
{
    // friend Singleton<ClassFactory>;

public:
    // class reflection
    void    registerClass(const std::string& className, create_object method) { m_class[className] = method; }
    Object* createClass(const std::string& className)
    {
        auto it = m_class.find(className);
        if (it == m_class.end())
            return nullptr;
        return it->second();
    }

    // field reflection
    void registerField(const std::string& className,
                       const std::string& fieldName,
                       const std::string& fieldType,
                       size_t             offset)
    {
        m_fields[className].push_back(new ClassField(fieldName, fieldType, offset));
    }
    ClassField* getClassField(const std::string& className, const std::string& fieldName)
    {
        for (auto* field : m_fields[className])
            if (field->name() == fieldName)
                return field;
        return nullptr;
    }

    // method reflection
    void registerMethod(const std::string& className, const std::string& methodName, uintptr_t method)
    {
        m_methods[className].push_back(new ClassMethod(methodName, method));
    }
    ClassMethod* getClassMethod(const std::string& className, const std::string& methodName)
    {
        for (auto* method : m_methods[className])
            if (method->name() == methodName)
                return method;
        return nullptr;
    }

private:
    std::map<std::string, create_object>             m_class;
    std::map<std::string, std::vector<ClassField*>>  m_fields;
    std::map<std::string, std::vector<ClassMethod*>> m_methods;
};

class Object
{
public:
    Object()          = default;
    virtual ~Object() = default;

    void               setClassName(const std::string& className) { m_classname = className; }
    const std::string& getClassName() const { return m_classname; }

    template<typename T>
    void get(const std::string& fieldName, T& value)
    {
        value =
            *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(this) +
                                  Singleton<ClassFactory>::instance()->getClassField(m_classname, fieldName)->offset());
    }
    template<typename T>
    void set(const std::string& fieldName, const T& value)
    {
        *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(this) +
                              Singleton<ClassFactory>::instance()->getClassField(m_classname, fieldName)->offset()) =
            value;
    }

    // TODO: 自动推导返回值类型
    // template<typename Ret, typename... Args>
    template<typename Ret = int, typename... Args>
    Ret call(const std::string& methodName, Args... args)
    {
        auto func_ptr = Singleton<ClassFactory>::instance()->getClassMethod(m_classname, methodName)->method();
        using Method  = std::function<Ret(decltype(this), Args...)>;
        return (*(reinterpret_cast<Method*>(func_ptr)))(this, args...);
    }

private:
    std::string m_classname;
};
