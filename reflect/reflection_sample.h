#pragma once

#include "../reflect/ClassRegister.h"

#include <iostream>

class ClassA : public Object
{
public:
    int fn(int x) const { return x + num; }

    int         num {114514};
    std::string name = "reflect";
};
REGISTER_CLASS(ClassA);
REGISTER_CLASS_FIELD(ClassA, num, int);
REGISTER_CLASS_FIELD(ClassA, name, std::string);
REGISTER_CLASS_METHOD(ClassA, fn, int, int);

inline void reflection_sample()
{
    auto* a = Singleton<ClassFactory>::instance()->createClass("ClassA");
    std::cout << a->getClassName() << "\n";
    int ans;
    a->get("num", ans);
    std::cout << ans << "\n";
    a->set("num", 1048596);
    a->get("num", ans);
    std::cout << ans << "\n";
    std::string name;
    a->get("name", name);
    std::cout << name << "\n";
    std::string str = "Steins::Gate";
    a->set("name", str);
    a->get("name", name);
    std::cout << name << "\n";
    ans = a->call("fn", 1);
    std::cout << ans << "\n";
}