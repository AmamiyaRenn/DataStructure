#pragma once

#include "lib/functional.h"
#include <functional>
#include <memory>
#include <string>

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
    void addListener(std::function<FuncType> listener) { listeners.push_back(listener); };
    void removeListener(uint32_t index) { listeners.erase(listeners.begin() + index); }
    void removeAllListeners() { listeners.clear(); }
    template<class... Arg>
    void fire(Arg... parameters)
    {
        for (auto listener : listeners) // Perfect forward the parameters to the listener
            listener(forward<Arg>(parameters)...);
    }

private:
    /// \brief      Keeps track of all the attached listeners.
    std::vector<std::function<FuncType>> listeners;
};