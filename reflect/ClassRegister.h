#pragma once

#include "pattern/singleton.h"
#include "reflect/ClassFactory.h"
#include <vadefs.h>

class ClassRegister
{
public:
    ClassRegister(const std::string& className, create_object method)
    {
        // register class
        Singleton<ClassFactory>::instance()->registerClass(className, method);
    }
    ClassRegister(const std::string& className,
                  const std::string& fieldName,
                  const std::string& fieldType,
                  size_t             offset)
    {
        // register class field
        Singleton<ClassFactory>::instance()->registerField(className, fieldName, fieldType, offset);
    }
    ClassRegister(const std::string& className, const std::string& methodName, uintptr_t method)
    {
        // register class method
        Singleton<ClassFactory>::instance()->registerMethod(className, methodName, method);
    }
};

#define REGISTER_CLASS(className) \
    Object* createObject##className() \
    { \
        Object* obj = new className(); \
        obj->setClassName(#className); \
        return obj; \
    } \
    ClassRegister ClassRegister##className(#className, createObject##className);

#define REGISTER_CLASS_FIELD(className, fieldName, fieldType) \
    ClassRegister ClassRegister##className##fieldName( \
        #className, #fieldName, #fieldType, reinterpret_cast<size_t>(&(static_cast<ClassA*>(nullptr)->fieldName)));

#define REGISTER_CLASS_METHOD(className, methodName, returnType, ...) \
    std::function<returnType(className*, ##__VA_ARGS__)> className##methodName##method = &className::methodName; \
    ClassRegister                                        ClassRegister##className##methodName( \
        #className, #methodName, reinterpret_cast<uintptr_t>(&className##methodName##method))