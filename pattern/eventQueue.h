#pragma once

#include "../DSA/List.h"
#include "../DSA/Vector.h"
#include "../lib/functional.h"

enum class EventType
{
    None,
    RunLine,
    Load,
    Fire,
};

template<typename FuncType>
class Event
{
public:
    void addListener(Function<FuncType> listener) { listeners.insert(listener); };
    template<class... Arg>
    void fire(EventType type, Arg... parameters)
    { // Perfect forward the parameters to the listener
        listeners[static_cast<int>(type)](forward<Arg>(parameters)...);
    }

private:
    Vector<Function<FuncType>> listeners;
};

using EventQueue = List<EventType>;
