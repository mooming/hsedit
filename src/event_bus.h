#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class EventBus {
public:
    using Handler = std::function<void(const std::string& data)>;
    
    // Subscribe to an event, returns handler ID for later unsubscription
    size_t subscribe(const std::string& event, Handler handler);
    
    // Publish an event
    void publish(const std::string& event, const std::string& data = "");
    
    // Unsubscribe by handler ID
    void unsubscribe(const std::string& event, size_t handler_id);
    
    // Clear all subscriptions
    void clear();
    
private:
    std::unordered_map<std::string, std::vector<Handler>> handlers_;
};

#endif  // EVENT_BUS_H
