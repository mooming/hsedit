#include "event_bus.h"
#include <iostream>

size_t EventBus::subscribe(const std::string& event, Handler handler) {
    size_t id = handlers_[event].size();
    handlers_[event].push_back(handler);
    return id;
}

void EventBus::publish(const std::string& event, const std::string& data) {
    auto it = handlers_.find(event);
    if (it != handlers_.end()) {
        for (const auto& handler : it->second) {
            handler(data);
        }
    }
}

void EventBus::unsubscribe(const std::string& event, size_t handler_id) {
    auto it = handlers_.find(event);
    if (it != handlers_.end()) {
        if (handler_id < it->second.size()) {
            it->second.erase(it->second.begin() + handler_id);
        }
    }
}

void EventBus::clear() {
    handlers_.clear();
}
