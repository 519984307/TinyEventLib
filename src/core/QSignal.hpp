#pragma once

#include "function_ref.hpp"
#include <vector>
#include <algorithm>

template<typename... Args>
class QSignal {
public:
    using SlotType = function_ref<void(Args...)>;

    void connect(SlotType slot) {
        slots.push_back(std::move(slot));
    }

    void disconnect(SlotType slot) {
        auto it = std::remove_if(slots.begin(), slots.end(),
                                 [&](const SlotType& other) {
                                     return other.target() == slot.target();
                                 });
        slots.erase(it, slots.end());
    }

    void operator()(Args... args) const {
        for (const auto& slot : slots) {
            slot(args...);
        }
    }

private:
    std::vector<SlotType> slots;
};

// Convenience function
template<typename... Args>
inline void connect(QSignal<Args...>& signal, typename QSignal<Args...>::SlotType slot) {
    signal.connect(std::move(slot));
}
